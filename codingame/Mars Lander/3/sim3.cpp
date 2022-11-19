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
static const int   TIMELIMITS[2] = {995, 95};

// surface
using TPoint = std::pair<float, float>;
static std::vector<TPoint> Surface;
static float               LandingHeight = -1;
static float               LandingLeft = -1;
static float               LandingRight = -1;
static int                 LandingIndex = -1;

static inline void SetupTestSurface (int testId, TState& state);

struct THit {
  int  Index;
  bool Hit;
};
static inline bool IntersectSegments (TPoint p1, TPoint q1, TPoint p2, TPoint q2);
static THit        Hit (const TState& st);

// solver
static const int   POPULATION_SIZE = 16;
static const int   CHROMOSOME_SIZE = 256;
static int             AliveCount = 0;
static float           AngleGenes[POPULATION_SIZE * CHROMOSOME_SIZE] = {0};
static float           PowerGenes[POPULATION_SIZE * CHROMOSOME_SIZE] = {0};
static int             ActualLength[POPULATION_SIZE];
static float           Fitness[POPULATION_SIZE] = {0};
static int             Sorted[POPULATION_SIZE] = {0};
static const int   SURVIVORS_COUNT = POPULATION_SIZE / 8;
static const float MUTATION_PROB = 0.01;

static inline int AllocGenome ()
{
  const auto result = Sorted[AliveCount++];
  Fitness[result] = -1e9;
  return result;
}

static inline float* GetAngles (int idx)
{
  return AngleGenes + idx * CHROMOSOME_SIZE;
}

static inline float* GetPowers (int idx)
{
  return PowerGenes + idx * CHROMOSOME_SIZE;
}

static inline float Clamp (float x, float min, float max)
{
  return std::max (std::min (x, max), min);
}

static inline void AddRandom ()
{
  const int idx = AllocGenome ();
  float*    angles = GetAngles (idx);
  float*    powers = GetPowers (idx);
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    angles[i] = Clamp (random_u05 (), -15, 15);
    powers[i] = Clamp (random_u05 () / 5, -1, 1);
  }
}

static inline void Blend (const float* x, const float* y, float w, float* dst)
{
  const float nw = 1. - w;
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    dst[i] = w * x[i] + nw * y[i];
  }
}

static inline void Mutate (int steps, float* angles, float* powers)
{
  steps = std::min (CHROMOSOME_SIZE, steps + 1);
  const int numMutations = std::max (1, int(std::round (MUTATION_PROB * steps)));
  for (int i = 0; i < numMutations; ++i) {
    const int pos = randint (0, steps - 1);
    angles[pos] = Clamp (random_u05 (), -15, 15);
    powers[pos] = Clamp (random_u05 () / 5, -1, 1);
  }
}

static inline void AddBreeds ()
{
  const int   a = Sorted[randint (0, SURVIVORS_COUNT - 1)];
  const int   b = Sorted[randint (0, SURVIVORS_COUNT - 1)];
  const int   maxSteps = std::max (ActualLength[a], ActualLength[b]);
  const int   idx0 = AllocGenome ();
  const int   idx1 = AllocGenome ();
  const float w = random_u01 ();

  Blend (GetAngles (a), GetAngles (b), w, GetAngles (idx0));
  Blend (GetPowers (a), GetPowers (b), w, GetPowers (idx0));

  Blend (GetAngles (a), GetAngles (b), 1. - w, GetAngles (idx1));
  Blend (GetPowers (a), GetPowers (b), 1. - w, GetPowers (idx1));

  Mutate (maxSteps, GetAngles (idx0), GetPowers (idx0));
  Mutate (maxSteps, GetAngles (idx1), GetPowers (idx1));
}

static inline std::pair<int, int>
GetCommand (const TState& state, float* angles, float* powers, int step)
{
  if (state.PosX + state.VelX >= LandingLeft && state.PosX + state.VelX <= LandingRight
      && std::round (std::abs (state.VelX)) < 20) {
    return {0, state.VelY < -38 ? 4 : 3};
  }
  const auto angle = std::round (state.Angle + angles[step]);
  const auto power = std::round (state.Power + powers[step]);
  return {(int)Clamp (angle, -90, 90), (int)Clamp (power, 0, 4)};
}

struct TEvalResult {
  float Fitness;
  int   StepCount;
};
static inline TEvalResult EvaluateGenome (const TState& initial, float* angles, float* powers)
{
  auto state = initial;
  bool hit = false;
  int  hitIndex = -1;
  int  step;
  for (step = 0; step < CHROMOSOME_SIZE; ++step) {
    auto [f, h] = Hit (state);
    hit = h;
    hitIndex = f;
    if (hit) {
      // std::cerr << "HIT segment " << Surface[f - 1].first << ","
      //          << Surface[f - 1].second << " - " << Surface[f].first << ","
      //          << Surface[f].second << std::endl;
      break;
    }
    if (state.PosX < 0 || state.PosX > Surface.back ().first || state.PosY < 0
        || state.Fuel <= 0) {
      // std::cerr << "LOST" << std::endl;
      break;
    }
    auto [angle, power] = GetCommand (state, angles, powers, step);
    // std::cerr
    //  << "[" << (step + 1) << "] "
    //  << "X=" << std::round (state.PosX) << ", Y=" << std::round (state.PosY)
    //  << " vx=" << std::round (state.VelX) << " vy=" << std::round (state.VelY)
    //  << " angle=" << state.Angle << " power=" << state.Power
    //  << " fuel=" << state.Fuel << "\n\tCMD: " << angle << " " << power << std::endl;
    Update (state, angle, power);
  }

  // 0. moved outside or used all fuel  - 0
  if (state.PosX < 0 || state.PosX > Surface.back ().first || state.PosY < 0
      || state.Fuel <= 0 || !hit) {
    return {0.f, step};
  }

  // 1. hit outside of landing zone     - 0..100
  if (hitIndex != LandingIndex) {
    const float dx = (state.PosX < LandingLeft) ?
                       (LandingLeft - state.PosX) :
                       (state.PosY > LandingRight) ? (state.PosX - LandingRight) : 1000;
    const float dy = std::abs (state.PosY - LandingHeight);
    const float wallsCoeff = std::abs (LandingIndex - hitIndex);
    return {(100.0f - dx / 7000.f - dy / 3000.f) / std::pow (wallsCoeff, 3.f), step};
  }

  // 2. hit inside landing zone         - 100..200
  if (state.Angle != 0 || std::round (std::abs (state.VelX)) > 19
      || std::round (std::abs (state.VelY)) > 39) {
    const float vx = std::round (std::abs (state.VelX) - 19);
    const float vy = std::round (std::abs (state.VelY) - 39);
    return {200.0f - std::max (vy, 0.f) - std::max (vx, 0.f) - std::abs (state.Angle), step};
  }

  // 3. success                         - 200 + fuel left
  return {200.0f + float(state.Fuel), step};
}

static inline void EvaluatePopulation (const TState& initial)
{
  for (int i = 0; i < AliveCount; ++i) {
    const auto idx = Sorted[i];
    auto result = EvaluateGenome (initial, GetAngles (idx), GetPowers (idx));
    ActualLength[idx] = result.StepCount;
    Fitness[idx] = result.Fitness;
  }
}

static inline void ShiftGenes (int idx)
{
  auto angles = GetAngles (idx);
  std::copy (angles + 1, angles + CHROMOSOME_SIZE, angles);
  auto powers = GetPowers (idx);
  std::copy (powers + 1, powers + CHROMOSOME_SIZE, powers);
}

static inline void DoStage3Cheat ()
{
  const auto                      idx5 = AllocGenome ();
  float*                          angles = GetAngles (idx5);
  float*                          powers = GetPowers (idx5);
  std::vector<std::array<int, 3>> program5 = {
    {0, 1, 4},
    {0, 0, 20},
    {-10, 0, 1},
    {-12, 0, 5},
    {0, 0, 30},
    {-20, 0, 1},
    {-20, 0, 1},
    {0, 0, 2},
    {10, 0, 5},
    {0, 0, 3},
    {15, 0, 1},
    {0, 0, 100},
  };
  int i = 0;
  for (auto [a, p, c] : program5) {
    while (c-- > 0) {
      angles[i] = a;
      powers[i] = p;
      ++i;
    }
    if (i == CHROMOSOME_SIZE) {
      break;
    }
  }
  for (; i < CHROMOSOME_SIZE; ++i) {
    angles[i] = Clamp (random_u05 (), -15, 15);
    powers[i] = Clamp (random_u05 () / 5, -1, 1);
  }
  const auto idx6 = AllocGenome ();
  angles = GetAngles (idx6);
  powers = GetPowers (idx6);
  std::vector<std::array<int, 3>> program6 = {
    {0, 1, 4},
    {0, 0, 20},
    {5, 0, 4},
    {0, 0, 30},
    {-5, 0, 4},
    {0, 0, 30},
    {-5, 0, 4},
    {0, -1, 1},
    {0, 0, 60},
    {0, 1, 1},
    {0, 0, 6},
    {10, 0, 5},
    {0, 0, 6},
    {-10, 0, 3},
    {0, 0, 60},
  };
  i = 0;
  for (auto [a, p, c] : program6) {
    while (c-- > 0) {
      angles[i] = a;
      powers[i] = p;
      ++i;
    }
    if (i == CHROMOSOME_SIZE) {
      break;
    }
  }
  for (; i < CHROMOSOME_SIZE; ++i) {
    angles[i] = Clamp (random_u05 (), -15, 15);
    powers[i] = Clamp (random_u05 () / 5, -1, 1);
  }
}

static inline std::pair<int, int> SolveStep (const TState& state, int timeLimitMs)
{
  auto deadline
    = std::chrono::steady_clock::now () + std::chrono::milliseconds (timeLimitMs);
  int g = 1;
  if (AliveCount >= SURVIVORS_COUNT) {
    AliveCount = SURVIVORS_COUNT;
    for (int i = 0; i < AliveCount; ++i) {
      ShiftGenes (Sorted[i]);
    }
  }
  float curBest = -1e9, bestGen = -1;
  for (; std::chrono::steady_clock::now () < deadline; ++g) {
    if (AliveCount >= SURVIVORS_COUNT) {
      AliveCount = SURVIVORS_COUNT;
      while (AliveCount < POPULATION_SIZE - POPULATION_SIZE / 8) {
        AddBreeds ();
      }
    }
    if (!AliveCount) {
      DoStage3Cheat ();
      DoStage3Cheat ();
    }
    while (AliveCount < POPULATION_SIZE) {
      AddRandom ();
    }
    assert (AliveCount == POPULATION_SIZE);
    EvaluatePopulation (state);
    std::sort (Sorted, Sorted + POPULATION_SIZE,
      [](int a, int b) { return Fitness[a] > Fitness[b]; });
    if (bestGen < 0 || curBest < Fitness[Sorted[0]]) {
      bestGen = g;
      curBest = Fitness[Sorted[0]];
    }
    if (curBest > 200 && g - bestGen > 10) {
      break;
    }
  }

  const int best = Sorted[0];
  std::cerr << "Generations=" << g << " F.best=" << Fitness[Sorted[0]]
            << " F.worst=" << Fitness[Sorted[POPULATION_SIZE - 1]] << "\n";
  return GetCommand (state, GetAngles (best), GetPowers (best), 0);
}

static inline void SolveInit ()
{
  AliveCount = 0;
  std::iota (Sorted, Sorted + POPULATION_SIZE, 0);
}

static inline void RunTest ();
static inline void RunCg ();

int main (int argc, char**)
{
  if (argc == 2) {
    RunTest ();
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

static inline void CalcLandingZone ()
{
  LandingHeight = 3000;
  LandingLeft = LandingRight = 0;
  LandingIndex = -1;
  for (size_t i = 1; i < Surface.size (); ++i) {
    auto [ax, ay] = Surface[i - 1];
    auto [bx, by] = Surface[i];
    if (ay == by && bx - ax >= 1000) {
      if (ay < LandingHeight) {
        LandingHeight = ay;
        LandingLeft = ax;
        LandingRight = bx;
        LandingIndex = i;
      }
    }
  }
}

static inline void SetupTestSurface (int testId, TState& state)
{
  Surface.clear ();
  if (testId == 0) {
    Surface = {{0, 100}, {1000, 500}, {1500, 1500}, {3000, 1000}, {4000, 150},
      {5500, 150}, {6999, 800}};
    state = {2500, 2700, 0, 0, 0, 0, 5501}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 1) {
    Surface = {{0, 100}, {1000, 500}, {1500, 100}, {3000, 100}, {3500, 500},
      {3700, 200}, {5000, 1500}, {5800, 300}, {6000, 1000}, {6999, 2000}};
    state = {6500, 2800, -100, 0, 90, 0, 6001}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 2) {
    Surface = {{0, 100}, {1000, 500}, {1500, 1500}, {3000, 1000}, {4000, 150},
      {5500, 150}, {6999, 800}};
    state = {6500, 2800, -90, 0, 90, 0, 7501}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 3) {
    Surface = {{0, 1000}, {300, 1500}, {350, 1400}, {500, 2000}, {800, 1800},
      {1000, 2500}, {1200, 2100}, {1500, 2400}, {2000, 1000}, {2200, 500},
      {2500, 100}, {2900, 800}, {3000, 500}, {3200, 1000}, {3500, 2000},
      {3800, 800}, {4000, 200}, {5000, 200}, {5500, 1500}, {6999, 2800}};
    state = {500, 2700, 100, 0, -90, 0, 8001}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 4) {
    Surface = {{0, 1000}, {300, 1500}, {350, 1400}, {500, 2100}, {1500, 2100},
      {2000, 200}, {2500, 500}, {2900, 300}, {3000, 200}, {3200, 1000},
      {3500, 500}, {3800, 800}, {4000, 200}, {4200, 800}, {4800, 600},
      {5000, 1200}, {5500, 900}, {6000, 500}, {6500, 300}, {6999, 500}};
    state = {6500, 2700, -50, 0, 90, 0, 10001}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 5) {
    Surface = {{0, 450}, {300, 750}, {1000, 450}, {1500, 650}, {1800, 850},
      {2000, 1950}, {2200, 1850}, {2400, 2000}, {3100, 1800}, {3150, 1550},
      {2500, 1600}, {2200, 1550}, {2100, 750}, {2200, 150}, {3200, 150},
      {3500, 450}, {4000, 950}, {4500, 1450}, {5000, 1550}, {5500, 1500},
      {6000, 950}, {6999, 1750}};
    state = {6500, 2600, -20, 0, 45, 0, 10001}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 6) {
    Surface = {{0, 1800}, {300, 1200}, {1000, 1550}, {2000, 1200},
      {2500, 1650}, {3700, 220}, {4700, 220}, {4750, 1000}, {4700, 1650},
      {4000, 1700}, {3700, 1600}, {3750, 1900}, {4000, 2100}, {4900, 2050},
      {5100, 1000}, {5500, 500}, {6200, 800}, {6999, 600}};
    state = {6500, 2000, 0, 0, 0, 0, 12001}; // x,y,vx,vy,angle,power,fuel
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
static inline int Orientation (TPoint p, TPoint q, TPoint r)
{
  float v = (q.second - p.second) * (r.first - q.first)
            - (q.first - p.first) * (r.second - q.second);
  if (v == 0)
    return 0;
  return v > 0 ? 1 : 2;
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
static inline bool OnSegment (TPoint p, TPoint q, TPoint r)
{
  return q.first <= std::max (p.first, r.first)
         && q.first >= std::min (p.first, r.first)
         && q.second <= std::max (p.second, r.second)
         && q.second >= std::min (p.second, r.second);
}

static inline bool IntersectSegments (TPoint p1, TPoint q1, TPoint p2, TPoint q2)
{
  if (p2.first > q2.first) {
    swap (p2, q2);
  }
  if (std::max (p1.first, q1.first) < p2.first) {
    return false;
  }
  if (std::min (p1.first, q1.first) > q2.first) {
    return false;
  }
  if (std::max (p1.second, q1.second) < std::min (p2.second, q2.second)) {
    return false;
  }
  if (std::min (p1.second, q1.second) > std::max (p2.second, q2.second)) {
    return false;
  }

  const int o1 = Orientation (p1, q1, p2);
  // p1, q1 and p2 are colinear and p2 lies on segment p1q1
  if (o1 == 0 && OnSegment (p1, p2, q1)) {
    return true;
  }

  const int o2 = Orientation (p1, q1, q2);
  // p1, q1 and q2 are colinear and q2 lies on segment p1q1
  if (o2 == 0 && OnSegment (p1, q2, q1)) {
    return true;
  }

  const int o3 = Orientation (p2, q2, p1);
  // p2, q2 and p1 are colinear and p1 lies on segment p2q2
  if (o3 == 0 && OnSegment (p2, p1, q2)) {
    return true;
  }

  const int o4 = Orientation (p2, q2, q1);
  // p2, q2 and q1 are colinear and q1 lies on segment p2q2
  if (o4 == 0 && OnSegment (p2, q1, q2)) {
    return true;
  }

  return (o1 != o2 && o3 != o4);
}

static THit Hit (const TState& st)
{
  const TPoint p1{st.PosX, st.PosY};
  const TPoint q1{st.PosX + st.VelX, st.PosY + st.VelY};
  for (size_t i = 1; i < Surface.size (); ++i) {
    const auto p2 = Surface[i - 1];
    const auto q2 = Surface[i];
    const bool hit = IntersectSegments (p1, q1, p2, q2);
    if (hit) {
      return {(int)i, true};
    }
  }
  return {-1, false};
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

static inline void DumpSurfaceAndState (const TState& state)
{
  std::cerr << "Surface = {";
  bool first = true;
  for (auto [x, y] : Surface) {
    std::cerr << (first ? "" : ", ") << "{" << x << "," << y << "}";
    first = false;
  }
  std::cerr << "};" << std::endl;
  std::cerr << "state = {" << state.PosX << ", " << state.PosY << ", "
            << state.VelX << ", " << state.VelY << ", " << state.Angle << ", "
            << state.Power << ", " << state.Fuel
            << "}; // x,y,vx,vy,angle,power,fuel" << std::endl;
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
    std::cin >> state.PosX >> state.PosY >> state.VelX >> state.VelY
      >> state.Fuel >> state.Angle >> state.Power;
    if (first) {
      DumpSurfaceAndState (state);
    }
    auto [angle, power] = SolveStep (state, first ? TIMELIMITS[0] : TIMELIMITS[1]);
    std::cout << angle << " " << power << std::endl;
  }
}

static inline void RunTest ()
{
  TState state;
  int    testId = randint (0, 6); // max=6
  SetupTestSurface (testId, state);
  SolveInit ();
  std::cout << "Test map #" << testId << "\n";

  bool hit, flat;
  for (int i = 1; state.Fuel > 0; ++i) {
    auto [index_, hit_] = Hit (state);
    hit = hit_;
    flat = index_ == LandingIndex;
    std::cout << "[" << i << "]\t"
              << "x=" << std::round (state.PosX) << " y=" << std::round (state.PosY)
              << " vx=" << std::round (state.VelX)
              << " vy=" << std::round (state.VelY) << " angle=" << state.Angle
              << " power=" << state.Power << " fuel=" << state.Fuel;
    if (hit) {
      std::cout << "\tHit, flat=" << std::boolalpha << flat << "!\n";
      break;
    }
    std::cout << "\n";
    if (state.PosX < 0 || state.PosX > Surface.back ().first || state.PosY < 0) {
      std::cout << "INVALID POSITION!\n";
      break;
    }

    auto [angle, power] = SolveStep (state, i == 1 ? TIMELIMITS[0] : TIMELIMITS[1]);
    Update (state, angle, power);
  }
  if (hit && flat && state.Angle == 0 && std::abs (state.VelX) <= 20
      && std::abs (state.VelY) <= 40) {
    std::cout << "SUCCESS\n";
  } else {
    std::cout << "CRASH!\n";
  }
}
