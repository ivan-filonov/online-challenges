#pragma GCC optimize("O3")

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

std::mt19937 rd{std::random_device{}()};

static inline float random_u01 ();
static inline float random_n05 ();
static inline float random_u05 ();
static inline int   randint (int min, int max);

// state
struct TState {
  float PosX = 0;
  float PosY = 0;
  float VelX = 0;
  float VelY = 0;
  int   Angle = 0;
  int   Power = 0;
  int   Fuel = 0;
};

static inline void UpdatePhysics (TState& state);
static inline void Update (TState& state, int angle, int power);
static inline void UpdateDelta (TState& state, int deltaAngle, int deltaPower);

static const float G = -3.711;

// surface
static std::vector<std::pair<float, float>> Surface;
static float                                LandingHeight = -1;
static float                                LandingLeft = -1;
static float                                LandingRight = -1;

void SetupTestSurface (int testId, TState& state);

std::pair<bool, bool> /*hit, flat*/ Hit (const TState& st);

// solver
static const int POPULATION_SIZE = 32;
static const int CHROMOSOME_SIZE = 128;
using TGenome = std::array<float, CHROMOSOME_SIZE>;
static int                                  AliveCount = 0;
static std::array<TGenome, POPULATION_SIZE> AngleGenes;
static std::array<TGenome, POPULATION_SIZE> PowerGenes;
static std::array<float, POPULATION_SIZE>   Fitness;
static std::array<int, POPULATION_SIZE>     Sorted;
static const int                        SURVIVORS_COUNT = POPULATION_SIZE / 4;
static const float                      MUTATION_PROB = 0.01;

static inline int AllocGenome ()
{
  const auto result = Sorted[AliveCount++];
  Fitness[result] = -1e9;
  return result;
}

static inline float Clamp (float x, float min, float max)
{
  return std::max (std::min (x, max), min);
}

static inline void AddRandom ()
{
  const int idx = AllocGenome ();
  float*    angles = AngleGenes[idx].data ();
  float*    powers = PowerGenes[idx].data ();
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    angles[i] = Clamp (random_u05 (), -15, 15);
    powers[i] = Clamp (random_u05 () / 5, -1, 1);
  }
}

static inline void Blend (TGenome& x, TGenome& y, float w, TGenome& dst)
{
  const float nw = 1. - w;
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    dst[i] = w * x[i] + nw * y[i];
  }
}

static inline void Mutate (TGenome& angles, TGenome& powers)
{
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    if (random_u01 () < MUTATION_PROB) {
      angles[i] = Clamp (random_u05 (), -15, 15);
      powers[i] = Clamp (random_u05 () / 5, -1, 1);
    }
  }
}

static inline void AddBreeds ()
{
  const int   a = Sorted[randint (0, SURVIVORS_COUNT - 1)];
  const int   b = Sorted[randint (0, SURVIVORS_COUNT - 1)];
  const int   idx0 = AllocGenome ();
  const int   idx1 = AllocGenome ();
  const float w = random_u01 ();
  const float nw = 1. - w;

  Blend (AngleGenes[a], AngleGenes[b], w, AngleGenes[idx0]);
  Blend (AngleGenes[a], AngleGenes[b], nw, AngleGenes[idx1]);
  Blend (PowerGenes[a], PowerGenes[b], w, PowerGenes[idx0]);
  Blend (PowerGenes[a], PowerGenes[b], nw, PowerGenes[idx1]);

  Mutate (AngleGenes[idx0], PowerGenes[idx0]);
  Mutate (AngleGenes[idx1], PowerGenes[idx1]);
}

static inline std::array<int, 2> GetCommand (const TState& state, TGenome& angles, TGenome& powers, int step)
{
  if (state.PosX + state.VelX >= LandingLeft && state.PosX + state.VelX <= LandingRight
      && std::round (std::abs (state.VelX)) < 20) {
    return {0, state.VelY < -38 ? 4 : 3};
  }
  return {(int)Clamp (std::round (state.Angle + angles[step]), -90, 90),
    (int)Clamp (std::round (state.Power + powers[step]), 0, 4)};
}

static inline float EvaluateGenome (const TState& initial, TGenome& angles, TGenome& powers)
{
  auto state = initial;
  bool hit = false, flat = false;
  for (int j = 0; j < CHROMOSOME_SIZE; ++j) {
    auto [h, f] = Hit (state);
    hit = h;
    flat = f;
    if (hit) {
      break;
    }
    if (state.PosX < 0 || state.PosX > Surface.back ().first || state.PosY < 0) {
      break;
    }
    auto [angle, power] = GetCommand (state, angles, powers, j);
    Update (state, angle, power);
  }
  float       v = (float)state.Fuel / 210;
  const float VX_PENALTY = 10;
  const float VY_PENALTY = 10;
  const int   XMARGIN = 5;
  if (flat) {
    v += VX_PENALTY * 19 + VY_PENALTY * 39;
  } else {
    int xdist = 0, ydist = 0;
    if (state.PosX < LandingLeft + XMARGIN) {
      xdist = LandingLeft - state.PosX;
    } else if (state.PosX > LandingRight - XMARGIN) {
      xdist = state.PosX - LandingRight;
    }
    ydist = std::abs (state.PosY - LandingHeight);
    v -= xdist * .5 + ydist;
  }
  v -= std::abs (state.Angle) * 50;
  auto vx = std::abs (std::round (state.VelX));
  auto vy = std::abs (std::round (state.VelY));
  if (vy > 39) {
    v -= VY_PENALTY * vy;
  }
  if (vx > 19) {
    v -= VX_PENALTY * vx;
  }
  return v;
}

static inline void EvaluatePopulation (const TState& initial)
{
  for (int i = 0; i < AliveCount; ++i) {
    const auto idx = Sorted[i];
    Fitness[idx] = EvaluateGenome (initial, AngleGenes[idx], PowerGenes[idx]);
  }
}

static inline void ShiftGenes (int idx)
{
  std::copy (AngleGenes[idx].begin () + 1, AngleGenes[idx].end (), AngleGenes[idx].begin ());
  std::copy (PowerGenes[idx].begin () + 1, PowerGenes[idx].end (), PowerGenes[idx].begin ());
}

static inline std::pair<int, int> SolveStep (const TState& state, int timeLimitMs)
{
  auto deadline = std::chrono::steady_clock::now () + std::chrono::milliseconds (timeLimitMs);
  int  g = 1;
  if (AliveCount >= SURVIVORS_COUNT) {
    AliveCount = SURVIVORS_COUNT;
    for (int i = 0; i < AliveCount; ++i) {
      ShiftGenes (Sorted[i]);
    }
  }
  for (; std::chrono::steady_clock::now () < deadline; ++g) {
    if (AliveCount >= SURVIVORS_COUNT) {
      AliveCount = SURVIVORS_COUNT;
      while (AliveCount < POPULATION_SIZE - POPULATION_SIZE / 8) {
        AddBreeds ();
      }
    }
    while (AliveCount < POPULATION_SIZE) {
      AddRandom ();
    }
    assert (AliveCount == POPULATION_SIZE);
    EvaluatePopulation (state);
    std::sort (Sorted.rbegin (), Sorted.rend (), [](int a, int b) { return Fitness[a] < Fitness[b]; });
  }

  const int best = Sorted[0];
  std::cerr << "Generations=" << g << " F.best=" << Fitness[Sorted.front ()]
            << " F.worst=" << Fitness[Sorted.back ()] << "\n";
  const auto [angle, power] = GetCommand (state, AngleGenes[best], PowerGenes[best], 0);
  return {angle, power};
}

static inline void SolveInit ()
{
  AliveCount = 0;
  std::iota (Sorted.begin (), Sorted.end (), 0);
}

static inline void RunTest (int testId);
static inline void RunCg ();

int main (int argc, char**)
{
  if (argc == 2) {
    RunTest (2); //(randint (0, 4)); // max 4
  } else {
    RunCg ();
  }
  return 0;
}

static inline void UpdatePhysics (TState& state)
{
  const auto  a = float(state.Angle) * 3.1415926 / 180;
  const float ax = -std::sin (a) * state.Power;
  const float ay = std::cos (a) * state.Power + G;
  state.PosX += state.VelX + ax / 2;
  state.PosY += state.VelY + ay / 2;
  state.VelX += ax;
  state.VelY += ay;
  state.Fuel -= state.Power * 10;
}

static inline void Update (TState& state, int angle, int power)
{
  state.Angle = std::max (std::min (angle, state.Angle + 15), state.Angle - 15);
  state.Angle = std::max (std::min (state.Angle, 90), -90);
  state.Power = std::max (std::min (power, state.Power + 1), state.Power - 1);
  state.Power = std::max (std::min (state.Power, 4), 0);
  UpdatePhysics (state);
}

static inline void UpdateDelta (TState& state, int deltaAngle, int deltaPower)
{
  Update (state, state.Angle + deltaAngle, state.Power + deltaPower);
}

void CalcLandingZone ()
{
  std::sort (Surface.begin (), Surface.end ());
  LandingHeight = 3000;
  LandingLeft = LandingRight = 0;
  for (size_t i = 1; i < Surface.size (); ++i) {
    auto [ax, ay] = Surface[i - 1];
    auto [bx, by] = Surface[i];
    if (ay == by && bx - ax >= 1000) {
      if (ay < LandingHeight) {
        LandingHeight = ay;
        LandingLeft = ax;
        LandingRight = bx;
      }
    }
  }
}

void SetupTestSurface (int testId, TState& state)
{
  Surface.clear ();
  if (testId == 0) {
    Surface = {{0, 100}, {1000, 500}, {1500, 1500}, {3000, 1000}, {4000, 150}, {5500, 150}, {6999, 800}};
    state = {2500, 2700, 0, 0, 0, 0, 5501}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 1) {
    Surface = {{0, 100}, {1000, 500}, {1500, 100}, {3000, 100}, {3500, 500}, {3700, 200},
      {5000, 1500}, {5800, 300}, {6000, 1000}, {6999, 2000}};
    state = {6500, 2800, -100, 0, 90, 0, 6001}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 2) {
    Surface = {{0, 100}, {1000, 500}, {1500, 1500}, {3000, 1000}, {4000, 150}, {5500, 150}, {6999, 800}};
    state = {6500, 2800, -90, 0, 90, 0, 7501}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 3) {
    Surface = {{0, 1000}, {300, 1500}, {350, 1400}, {500, 2000}, {800, 1800}, {1000, 2500},
      {1200, 2100}, {1500, 2400}, {2000, 1000}, {2200, 500}, {2500, 100}, {2900, 800}, {3000, 500},
      {3200, 1000}, {3500, 2000}, {3800, 800}, {4000, 200}, {5000, 200}, {5500, 1500}, {6999, 2800}};
    state = {500, 2700, 100, 0, -90, 0, 8001}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 4) {
    Surface = {{0, 1000}, {300, 1500}, {350, 1400}, {500, 2100}, {1500, 2100}, {2000, 200},
      {2500, 500}, {2900, 300}, {3000, 200}, {3200, 1000}, {3500, 500}, {3800, 800}, {4000, 200},
      {4200, 800}, {4800, 600}, {5000, 1200}, {5500, 900}, {6000, 500}, {6500, 300}, {6999, 500}};
    state = {6500, 2700, -50, 0, 90, 0, 10001}; // x,y,vx,vy,angle,power,fuel
  } else {
    throw std::exception{};
  }
  CalcLandingZone ();
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
static inline int Orientation (std::pair<float, float> p, std::pair<float, float> q, std::pair<float, float> r)
{
  float v = (q.second - p.second) * (r.first - q.first) - (q.first - p.first) * (r.second - q.second);
  if (v == 0)
    return 0;
  return v > 0 ? 1 : 2;
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
static inline bool OnSegment (std::pair<float, float> p, std::pair<float, float> q, std::pair<float, float> r)
{
  return q.first <= std::max (p.first, r.first) && q.first >= std::min (p.first, r.first)
         && q.second <= std::max (p.second, r.second) && q.second >= std::min (p.second, r.second);
}

std::pair<bool, bool> /*hit, flat*/ Hit (const TState& st)
{
  const std::pair<float, float> p1{st.PosX, st.PosY};
  const std::pair<float, float> q1{st.PosX + st.VelX, st.PosY + st.VelY};
  for (size_t i = 1; i < Surface.size (); ++i) {
    const auto p2 = Surface[i - 1];
    const auto q2 = Surface[i];
    assert (p2.first <= q2.first);
    if (std::max (p1.first, q1.first) < p2.first) {
      continue;
    }
    if (std::min (p1.first, q1.first) > q2.first) {
      continue;
    }
    if (std::max (p1.second, q1.second) < std::min (p2.second, q2.second)) {
      continue;
    }
    if (std::min (p1.second, q1.second) > std::max (p2.second, q2.second)) {
      continue;
    }
    const int o1 = Orientation (p1, q1, p2);
    const int o2 = Orientation (p1, q1, q2);
    const int o3 = Orientation (p2, q2, p1);
    const int o4 = Orientation (p2, q2, q1);
    if (o1 != o2 && o3 != o4) {
      return {true, p2.second == q2.second && st.VelY < 0};
    }

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && OnSegment (p1, p2, q1)) {
      return {true, p2.second == q2.second && st.VelY < 0};
    }
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && OnSegment (p1, q2, q1)) {
      return {true, p2.second == q2.second && st.VelY < 0};
    }
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && OnSegment (p2, p1, q2)) {
      return {true, p2.second == q2.second && st.VelY < 0};
    }
    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && OnSegment (p2, q1, q2)) {
      return {true, p2.second == q2.second && st.VelY < 0};
    }
  }
  return {false, false};
}

static inline float random_u01 ()
{
  return std::uniform_real_distribution<float> (0, 1) (rd);
}

static inline float random_n05 ()
{
  return std::normal_distribution<float> (0, 5) (rd);
}

static inline float random_u05 ()
{
  return std::uniform_real_distribution<float> (-15, 15) (rd);
}

static inline int randint (int min, int max)
{
  return std::uniform_int_distribution<int> (min, max) (rd);
}

static inline void RunCg ()
{
  TState state;
  // read surface
  int n;
  std::cin >> n;
  std::cin.ignore ();
  Surface.resize (n);
  for (int i = 0; i < n; ++i) {
    std::cin >> Surface[i].first >> Surface[i].second;
    std::cin.ignore ();
  }
  CalcLandingZone ();

  SolveInit ();
  for (bool first = true;; first = false) {
    std::cin >> state.PosX >> state.PosY >> state.VelX >> state.VelY >> state.Fuel >> state.Angle
      >> state.Power;
    auto [angle, power] = SolveStep (state, first ? 995 : 95);
    std::cout << angle << " " << power << std::endl;
  }
}

static inline void RunTest (int testId)
{
  TState state;
  SetupTestSurface (testId, state);
  SolveInit ();
  std::cout << "Test map #" << testId << "\n";

  bool hit, flat;
  for (int i = 1; state.Fuel > 0; ++i) {
    auto [hit_, flat_] = Hit (state);
    hit = hit_;
    flat = flat_;
    std::cout << "[" << i << "]\t"
              << "x=" << std::round (state.PosX) << " y=" << std::round (state.PosY)
              << " vx=" << std::round (state.VelX) << " vy=" << std::round (state.VelY)
              << " angle=" << state.Angle << " power=" << state.Power << " fuel=" << state.Fuel;
    if (hit) {
      std::cout << "\tHit, flat=" << std::boolalpha << flat << "!\n";
      break;
    }
    std::cout << "\n";
    if (state.PosX < 0 || state.PosX > Surface.back ().first || state.PosY < 0) {
      std::cout << "INVALID POSITION!\n";
      break;
    }

    auto [angle, power] = SolveStep (state, i == 1 ? 995 : 95);
    Update (state, angle, power);
  }
  if (hit && flat && state.Angle == 0 && std::abs (state.VelX) <= 20 && std::abs (state.VelY) <= 40) {
    std::cout << "SUCCESS\n";
  } else {
    std::cout << "CRASH!\n";
  }
}
