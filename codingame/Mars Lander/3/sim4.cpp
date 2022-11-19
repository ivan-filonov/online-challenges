#pragma GCC optimize("O2", "inline", "omit-frame-pointer")

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <vector>

struct TState {
  float x;
  float y;
  float vx;
  float vy;
  int   angle;
  int   power;
  int   fuel;
};

struct TPoint {
  int x;
  int y;
};

struct THit {
  int  Index; // segment is ([Index-1], [Index])
  bool Hit;
};

struct TCmd {
  int Angle;
  int Power;
};

static const float G = -3.711;
static const int   TIMELIMITS[2] = {993, 93};

static float SinTable[181]; //[-90..90]
static float CosTable[181]; //[-90..90]

static std::mt19937 rd{std::random_device{}()};

static inline float random_u01 ();
static inline float random_n05 ();
static inline float random_u15 ();
static inline int   randint (int min, int max);

static inline int Clamp (int x, int min, int max);

static inline void        UpdatePhysics (TState& state);
static inline void        Update (TState& state, TCmd cmd);
static inline std::string ToString (const TState& state);

namespace Surface
{
static TPoint* Points;
static int     NumPoints = 0;

static int LandingHeight = -1;
static int LandingLeft = -1;
static int LandingRight = -1;
static int LandingIndex = -1;

static inline void SetupTestSurface (int testId, TState& state);
static inline THit Hit (const TState& st);
static inline THit Hit (int x1, int y1, int x2, int y2);
static inline void ReadFromStdin ();
static inline void Dump (const TState& state);
} // namespace Surface

static inline std::chrono::steady_clock::time_point Now ();
static inline std::chrono::steady_clock::duration   Ms (int ms);

namespace GA
{
struct TEvalResult {
  float fitness;
  int   steps;
};

static const int CHROMOSOME_SIZE = 256;
static const int POPULATION_SIZE = 32;
static const int SURVIVORS_COUNT = POPULATION_SIZE / 8;
static const int RANDOM_COUNT = POPULATION_SIZE / 8;
static const int BLEND_SCALE = 256;
static const int MUTATIONS_COUNT = 1;

static const float FITNESS_STEP = 1000;

static int    AliveCount;
static int    Sorted[POPULATION_SIZE];
static float  Fitness[POPULATION_SIZE];
static int8_t RotationGenes[POPULATION_SIZE * CHROMOSOME_SIZE];
static int8_t ThrustGenes[POPULATION_SIZE * CHROMOSOME_SIZE];
static int    ActualLength[POPULATION_SIZE];

static inline int AllocGenome ()
{
  const int result = Sorted[AliveCount];
  Fitness[result] = -1e9;
  AliveCount += 1;
  return result;
}

static inline int8_t* Rotations (int idx)
{
  return RotationGenes + CHROMOSOME_SIZE * idx;
}

static inline int8_t* Thrusts (int idx)
{
  return ThrustGenes + CHROMOSOME_SIZE * idx;
}

static inline void ShiftGenes (int idx)
{
  if (ActualLength[idx] > 0) {
    ActualLength[idx] -= 1;
  }
  auto p = Rotations (idx);
  std::copy (p + 1, p + CHROMOSOME_SIZE, p);
  p = Thrusts (idx);
  std::copy (p + 1, p + CHROMOSOME_SIZE, p);
}

static inline TCmd
GetCommand (const TState& state, const int8_t* rotations, const int8_t* thrusts, int step)
{
  TCmd cmd;
  if (state.y < Surface::LandingHeight + 100 && state.x + state.vx >= Surface::LandingLeft
      && state.x + state.vx <= Surface::LandingRight
      && std::abs (std::round (state.vx)) < 19) {
    cmd.Angle = 0;
    cmd.Power = std::round (state.vy) < -39 ? 4 : 3;
  } else {
    cmd.Angle = Clamp (state.angle + rotations[step], -90, 90);
    cmd.Power = Clamp (state.power + thrusts[step], 0, 4);
  }
  return cmd;
}

static inline void
Blend (const int8_t* x, const int k, const int8_t* y, int8_t* dst, int min, int max)
{
  const int nk = BLEND_SCALE - k;
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    const int xi = x[i];
    const int yi = y[i];
    dst[i] = Clamp ((xi * k + yi * nk) / BLEND_SCALE, min, max);
  }
}

static inline void Mutate (int8_t* rotations, int8_t* thrusts, int range)
{
  for (int i = 0; i < MUTATIONS_COUNT; ++i) {
    int idx = randint (0, range);
    rotations[idx] = randint (-15, 15);
    thrusts[idx] = randint (-1, 1);
  }
}

static inline void AddBreeds ()
{
  const int a = Sorted[randint (0, SURVIVORS_COUNT - 1)];
  const int b = Sorted[randint (0, SURVIVORS_COUNT - 1)];
  const int mutationRange
    = Clamp (ActualLength[a] > ActualLength[b] ? ActualLength[b] : ActualLength[a],
      0, CHROMOSOME_SIZE - 1);
  const int na = AllocGenome ();
  const int nb = AllocGenome ();

  const float w = randint (BLEND_SCALE * 3 / 15, BLEND_SCALE * 12 / 15);

  Blend (Rotations (a), w, Rotations (b), Rotations (na), -15, 15);
  Blend (Rotations (a), BLEND_SCALE - w, Rotations (b), Rotations (nb), -15, 15);

  Blend (Thrusts (a), w, Thrusts (b), Thrusts (na), -1, 1);
  Blend (Thrusts (a), BLEND_SCALE - w, Thrusts (b), Thrusts (nb), -1, 1);

  Mutate (Rotations (na), Thrusts (na), mutationRange);
  Mutate (Rotations (nb), Thrusts (nb), mutationRange);
}

static inline void AddRandom (const TState& state)
{
  const int idx = AllocGenome ();
  auto      rotations = Rotations (idx);
  auto      thrusts = Thrusts (idx);
  int       angle = state.angle;
  int       power = state.power;
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    const int min = angle < -75 ? -90 : (angle - 15);
    const int max = angle > 75 ? 90 : (angle + 15);
    const int val = randint (min, max);
    rotations[i] = val - angle;
    angle = val;
  }
  for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
    const int min = (power > 0) ? power - 1 : 0;
    const int max = (power < 4) ? power + 1 : 4;
    const int val = randint (min, max);
    thrusts[i] = val - power;
    power = val;
  }
}

static inline TEvalResult EvaluateGenome (TState state /*copy on stack*/,
  const int8_t*                                  rotations,
  const int8_t*                                  thrusts)
{
  THit hit;
  int  step;
  int  x = std::round (state.x);
  int  y = std::round (state.y);
  for (step = 0; step < CHROMOSOME_SIZE; ++step) {
    Update (state, GetCommand (state, rotations, thrusts, step));
    int nx = std::round (state.x);
    int ny = std::round (state.y);
    hit = Surface::Hit (x, y, nx, ny);
    x = nx;
    y = ny;
    if (hit.Hit) {
      break;
    }
    if (state.x < 0 || state.x > 6999 || state.y < 0 || state.fuel <= 0) {
      break;
    }
  }

  TEvalResult eval;
  eval.steps = step;
  // moved outside or ran out of fuel -> 0
  if (!hit.Hit) {
    eval.fitness = 0;
  }
  // hit outside of landing zone -> 0..100
  else if (hit.Index != Surface::LandingIndex) {
    float dx = 1000;
    if (state.x < Surface::LandingLeft) {
      dx = Surface::LandingLeft - state.x;
    } else if (state.x > Surface::LandingRight) {
      dx = state.x - Surface::LandingRight;
    }
    float dy = std::abs (state.y - Surface::LandingHeight);
    float di = std::abs (hit.Index - Surface::LandingIndex);
    float vx = std::abs (state.vx);
    float vy = std::abs (state.vy);
    eval.fitness = (FITNESS_STEP - dx / 7000.f - dy / 3000.f - vx / 50.f - vy / 50.f)
                   / std::pow (di, 1.f);
  }
  // crash inside landing zone -> 100..200
  else if (state.angle != 0 || std::round (std::abs (state.vx)) > 19 || state.vy < -39) {
    float da = std::abs (state.angle); //-90, 90
    float dvx = std::abs (state.vx) - 19;
    if (dvx < 0) {
      dvx = 0;
    }
    float dvy = std::abs (state.vy) - 39;
    if (dvy < 0) {
      dvy = 0;
    }
    eval.fitness = FITNESS_STEP * 2 - (da + dvx + dvy);
  }
  // success -> 200 + fuel
  else {
    eval.fitness = FITNESS_STEP * 2 + state.fuel;
  }

  return eval;
}

static inline void EvaluatePopulation (const TState& state)
{
  for (int i = 0; i < AliveCount; ++i) {
    const int  idx = Sorted[i];
    const auto eval = EvaluateGenome (state, Rotations (idx), Thrusts (idx));
    Fitness[idx] = eval.fitness;
    ActualLength[idx] = eval.steps;
  }
}

static inline int CompareGenes (int a, int b)
{
  return Fitness[a] > Fitness[b];
}

static inline TCmd SolverStep (const TState& state, int timeLimitMs)
{
  (void)state;
  const auto deadline = Now () + Ms (timeLimitMs);
  if (AliveCount >= SURVIVORS_COUNT) {
    AliveCount = SURVIVORS_COUNT;
  }
  for (int i = 0; i < AliveCount; ++i) {
    ShiftGenes (Sorted[i]);
  }
  int   generation = 1;
  int   bestGen = -1;
  float bestScore = -1e9;
  for (; Now () < deadline; ++generation) {
    if (AliveCount >= SURVIVORS_COUNT) {
      while (AliveCount < POPULATION_SIZE - RANDOM_COUNT) {
        AddBreeds ();
      }
    }
    while (AliveCount < POPULATION_SIZE) {
      AddRandom (state);
    }
    EvaluatePopulation (state);

    std::sort (Sorted, Sorted + POPULATION_SIZE, CompareGenes);
#if 0
    for (int i = 1; i < AliveCount; ++i) {
      auto si1 = Sorted[i - 1];
      auto si = Sorted[i];
      if (Fitness[si1] < Fitness[si]) {
        std::cerr << "i=" << i << " Sorted[i-1]=" << si1 << " Sorted[i]=" << si
                  << " Fitness[" << si1 << "]=" << Fitness[si1] << " Fitness["
                  << si << "]=" << Fitness[si] << std::endl;
      }
      assert (Fitness[Sorted[i - 1]] >= Fitness[Sorted[i]]);
    }
#endif
    if (bestGen < 0 || bestScore < Fitness[Sorted[0]]) {
      bestGen = generation;
      bestScore = Fitness[Sorted[0]];
    }
    if (true && bestScore > FITNESS_STEP * 2 && generation - bestGen > 10) {
      break;
    }
  }
  const auto best = Sorted[0];
  const auto worst = Sorted[AliveCount - 1];

  auto cmd = GetCommand (state, Rotations (best), Thrusts (best), 0);
  std::cerr << "Generations=" << generation << ", F.best=" << Fitness[best]
            << ", F.worst=" << Fitness[worst] << "\tcmd: angle=" << cmd.Angle
            << " power=" << cmd.Power << std::endl;
  if (Fitness[best] < FITNESS_STEP) {
    cmd.Angle = 12;
    cmd.Power = 4;
  }
  return cmd;
}

static inline void SolverInit (const TState&)
{
  AliveCount = 0;
  std::iota (Sorted, Sorted + POPULATION_SIZE, 0);
}
} // namespace GA

static inline void SolverInit (const TState& state);
static inline TCmd SolverStep (const TState& state, int timeLimitMs);

namespace DummySolver
{
static inline void SolverInit (const TState&)
{
}

static inline TCmd SolverStep (const TState& state, int /*timeLimitMs*/)
{
  TCmd cmd;
  cmd.Angle = 0;
  cmd.Power = state.vy > -39 ? 3 : 4;
  return cmd;
}
} // namespace DummySolver

static inline void SolverInit (const TState& state)
{
  // DummySolver::SolverInit (state);
  GA::SolverInit (state);
}

static inline TCmd SolverStep (const TState& state, int timeLimitMs)
{
  // return DummySolver::SolverStep (state, timeLimitMs);
  return GA::SolverStep (state, timeLimitMs);
}

static void RunTest ()
{
  const int testId = randint (5, 6); // max=6
  std::cout << "Test map #" << testId << "\n";
  TState state;
  Surface::SetupTestSurface (testId, state);
  SolverInit (state);
  THit hit;
#define DEBUG_HISTORY 0
#if DEBUG_HISTORY
  std::vector<TCmd>                    history;
  std::vector<std::pair<float, float>> points;
#endif

  bool flat;
  for (int i = 1; state.fuel > 0; ++i) {
#if DEBUG_HISTORY
    points.emplace_back (state.x, state.y);
#endif
    hit = Surface::Hit (state);
    flat = hit.Index == Surface::LandingIndex;
    std::cout << "[" << i << "]\t"
              << "x=" << std::round (state.x) << " y=" << std::round (state.y)
              << " vx=" << std::round (state.vx)
              << " vy=" << std::round (state.vy) << " angle=" << state.angle
              << " power=" << state.power << " fuel=" << state.fuel;
    if (hit.Hit) {
      std::cout << "\tHit, flat=" << std::boolalpha << flat << "!\n";
      break;
    }
    std::cout << "\n";
    if (state.x < 0 || state.x > 6999 || state.y < 0) {
      std::cout << "INVALID POSITION!\n";
      break;
    }

    auto cmd = SolverStep (state, TIMELIMITS[i == 1 ? 0 : 1]);
    Update (state, cmd);
#if DEBUG_HISTORY
    history.push_back (cmd);
#endif
  }

  if (hit.Hit && flat && state.angle == 0 && std::abs (state.vx) < 20
      && std::abs (state.vy) < 40) {
    std::cout << "SUCCESS\n";
  } else {
    std::cout << "CRASH!\n";
  }

#if DEBUG_HISTORY
  std::cout << "// test #" << testId << "\n{";
  for (auto cmd : history) {
    std::cout << "{" << cmd.Angle << "," << cmd.Power << "}, ";
  }
  std::cout << "};\n";
  std::cout << "// trajectory: [";
  for (auto [x, y] : points) {
    std::cout << "(" << std::round (x) << ", " << std::round (y) << "), ";
  }
  std::cout << "]\n";
#endif
}

static void RunCg ()
{
  TState state;
  TState expected;
  Surface::ReadFromStdin ();

  for (bool first = true;; first = false) {
    std::cin >> state.x >> state.y >> state.vx >> state.vy >> state.fuel
      >> state.angle >> state.power;
    if (first) {
      expected = state;
      Surface::Dump (state);
      SolverInit (state);
    } else {
#define C(fld) ((int)std::round (state.fld) != (int)std::round (expected.fld))
      if (C (x) || C (y) || C (vx) || C (vy) || C (angle) || C (power) || C (fuel)) {
        std::cerr << "Invalid state: " << ToString (state)
                  << " expected: " << ToString (expected) << std::endl;
        std::exit (0);
      }
#undef C
    }
    const auto cmd = SolverStep (expected, TIMELIMITS[first ? 0 : 1]);
    std::cout << cmd.Angle << " " << cmd.Power << std::endl;
    Update (expected, cmd);
  }
}

static inline std::string ToString (const TState& state)
{
  std::ostringstream ss;
  ss << "{.x=" << state.x << ", .y=" << state.y << ", .vx=" << state.vx
     << ", .vy=" << state.vy << ", .angle=" << state.angle
     << ", .power=" << state.power << ", .fuel=" << state.fuel << "}";
  return ss.str ();
}

static inline int Clamp (int x, int min, int max)
{
  return x < min ? min : x > max ? max : x;
}

static inline void Update (TState& state, TCmd cmd)
{
  state.angle = Clamp (Clamp (cmd.Angle, state.angle - 15, state.angle + 15), -90, 90);
  state.power = Clamp (Clamp (cmd.Power, state.power - 1, state.power + 1), 0, 4);

  UpdatePhysics (state);
}

static inline void UpdatePhysics (TState& state)
{
  const float ax = -SinTable[state.angle + 90] * state.power;
  const float ay = CosTable[state.angle + 90] * state.power + G;
  state.x += state.vx + ax / 2;
  state.y += state.vy + ay / 2;
  state.vx += ax;
  state.vy += ay;
  state.fuel -= state.power;
}

static inline float random_u01 ()
{
  return std::uniform_real_distribution<float> (0, 1) (rd);
}

static inline float random_n05 ()
{
  return std::normal_distribution<float> (0, 5) (rd);
}

static inline float random_u15 ()
{
  return std::uniform_real_distribution<float> (-15, 15) (rd);
}

static inline int randint (int min, int max)
{
  return std::uniform_int_distribution<int> (min, max) (rd);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
static inline int Orientation (int px, int py, int qx, int qy, int rx, int ry)
{
  const int v = (qy - py) * (rx - qx) - (qx - px) * (ry - qy);
  if (v == 0) {
    return 0;
  }
  return v > 0 ? 1 : 2;
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
static inline bool OnSegment (int px, int py, int qx, int qy, int rx, int ry)
{
  auto [left, right] = std::minmax (px, rx);
  auto [top, bottom] = std::minmax (py, ry);
  return qx <= right && qx >= left && qy <= bottom && qy >= top;
}

static inline bool
IntersectSegments (int p1x, int p1y, int q1x, int q1y, int p2x, int p2y, int q2x, int q2y)
{
  auto [left1, right1] = std::minmax (p1x, q1x);
  auto [left2, right2] = std::minmax (p2x, q2x);
  if (right1 < left2 || right2 < left1) {
    return false;
  }
  auto [top1, bottom1] = std::minmax (p1y, q1y);
  auto [top2, bottom2] = std::minmax (p2y, q2y);
  if (bottom1 < top2 || bottom2 < top1) {
    return false;
  }

  const int o1 = Orientation (p1x, p1y, q1x, q1y, p2x, p2y);
  // p1, q1 and p2 are colinear and p2 lies on segment p1q1
  if (o1 == 0 && OnSegment (p1x, p1y, p2x, p2y, q1x, q1y)) {
    return true;
  }

  const int o2 = Orientation (p1x, p1y, q1x, q1y, q2x, q2y);
  // p1, q1 and q2 are colinear and q2 lies on segment p1q1
  if (o2 == 0 && OnSegment (p1x, p1y, q2x, q2y, q1x, q1y)) {
    return true;
  }

  const int o3 = Orientation (p2x, p2y, q2x, q2y, p1x, p1y);
  // p2, q2 and p1 are colinear and p1 lies on segment p2q2
  if (o3 == 0 && OnSegment (p2x, p2y, p1x, p1y, q2x, q2y)) {
    return true;
  }

  const int o4 = Orientation (p2x, p2y, q2x, q2y, q1x, q1y);
  // p2, q2 and q1 are colinear and q1 lies on segment p2q2
  if (o4 == 0 && OnSegment (p2x, p2y, q1x, q1y, q2x, q2y)) {
    return true;
  }

  return (o1 != o2 && o3 != o4);
}

namespace Surface
{
static inline THit Hit (const TState& st)
{
  const int   p1x = std::round (st.x);
  const int   p1y = std::round (st.y);
  const float ax = -SinTable[st.angle + 90] * st.power;
  const float ay = CosTable[st.angle + 90] * st.power + G;
  const int   q1x = std::round (st.x + st.vx + ax / 2);
  const int   q1y = std::round (st.y + st.vy + ay / 2);
  THit        result;
  result.Hit = false;
  for (int i = 1; i < NumPoints; ++i) {
    const auto p2 = Points[i - 1];
    const auto q2 = Points[i];
    const bool hit = IntersectSegments (p1x, p1y, q1x, q1y, p2.x, p2.y, q2.x, q2.y);
    if (hit) {
      result.Hit = true;
      result.Index = i;
      break;
    }
  }
  return result;
}

static inline THit Hit (int x1, int y1, int x2, int y2)
{
  THit result;
  result.Hit = false;
  for (int i = 1; i < NumPoints; ++i) {
    const auto p2 = Points[i - 1];
    const auto q2 = Points[i];
    const bool hit = IntersectSegments (x1, y1, x2, y2, p2.x, p2.y, q2.x, q2.y);
    if (hit) {
      result.Hit = true;
      result.Index = i;
      break;
    }
  }
  return result;
}

static inline void CalcLandingZone ()
{
  LandingHeight = 3000;
  LandingLeft = LandingRight = 0;
  LandingIndex = -1;
  for (int i = 1; i < NumPoints; ++i) {
    auto a = Points[i - 1];
    auto b = Points[i];
    if (a.y == b.y && b.x - a.x >= 1000) {
      LandingHeight = a.y;
      LandingLeft = std::min (a.x, b.x);
      LandingRight = std::max (a.x, b.x);
      LandingIndex = i;
    }
  }
  assert (LandingIndex > 0);
}

static inline void FillPoints (std::initializer_list<TPoint>&& src)
{
  NumPoints = src.size ();
  Points = new TPoint[NumPoints];
  auto src_it = src.begin ();
  auto dst_it = Points;
  for (int i = 0; i < NumPoints; ++i) {
    *dst_it++ = *src_it++;
  }
}

static inline void SetupTestSurface (int testId, TState& state)
{
  if (testId == 0) {
    FillPoints ({{0, 100}, {1000, 500}, {1500, 1500}, {3000, 1000},
      {4000, 150}, {5500, 150}, {6999, 800}});
    state = {2500, 2700, 0, 0, 0, 0, 550}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 1) {
    FillPoints ({{0, 100}, {1000, 500}, {1500, 100}, {3000, 100}, {3500, 500},
      {3700, 200}, {5000, 1500}, {5800, 300}, {6000, 1000}, {6999, 2000}});
    state = {6500, 2800, -100, 0, 90, 0, 600}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 2) {
    FillPoints ({{0, 100}, {1000, 500}, {1500, 1500}, {3000, 1000},
      {4000, 150}, {5500, 150}, {6999, 800}});
    state = {6500, 2800, -90, 0, 90, 0, 750}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 3) {
    FillPoints ({{0, 1000}, {300, 1500}, {350, 1400}, {500, 2000}, {800, 1800},
      {1000, 2500}, {1200, 2100}, {1500, 2400}, {2000, 1000}, {2200, 500},
      {2500, 100}, {2900, 800}, {3000, 500}, {3200, 1000}, {3500, 2000},
      {3800, 800}, {4000, 200}, {5000, 200}, {5500, 1500}, {6999, 2800}});
    state = {500, 2700, 100, 0, -90, 0, 800}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 4) {
    FillPoints ({{0, 1000}, {300, 1500}, {350, 1400}, {500, 2100}, {1500, 2100},
      {2000, 200}, {2500, 500}, {2900, 300}, {3000, 200}, {3200, 1000},
      {3500, 500}, {3800, 800}, {4000, 200}, {4200, 800}, {4800, 600},
      {5000, 1200}, {5500, 900}, {6000, 500}, {6500, 300}, {6999, 500}});
    state = {6500, 2700, -50, 0, 90, 0, 1000}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 5) {
    FillPoints ({{0, 450}, {300, 750}, {1000, 450}, {1500, 650}, {1800, 850},
      {2000, 1950}, {2200, 1850}, {2400, 2000}, {3100, 1800}, {3150, 1550}, {2500, 1600},
      {2200, 1550}, {2100, 750}, {2200, 150}, {3200, 150}, {3500, 450}, {4000, 950},
      {4500, 1450}, {5000, 1550}, {5500, 1500}, {6000, 950}, {6999, 1750}});
    state = {6500, 2600, -20, 0, 45, 0, 1000}; // x,y,vx,vy,angle,power,fuel
  } else if (testId == 6) {
    FillPoints ({{0, 1800}, {300, 1200}, {1000, 1550}, {2000, 1200},
      {2500, 1650}, {3700, 220}, {4700, 220}, {4750, 1000}, {4700, 1650},
      {4000, 1700}, {3700, 1600}, {3750, 1900}, {4000, 2100}, {4900, 2050},
      {5100, 1000}, {5500, 500}, {6200, 800}, {6999, 600}});
    state = {6500, 2000, 0, 0, 0, 0, 1200}; // x,y,vx,vy,angle,power,fuel
  } else {
    throw std::exception{};
  }
  CalcLandingZone ();
}

static inline void ReadFromStdin ()
{
  std::cin >> NumPoints;
  std::cin.ignore ();
  Points = new TPoint[NumPoints];
  for (int i = 0; i < NumPoints; ++i) {
    std::cin >> Points[i].x >> Points[i].y;
    std::cin.ignore ();
  }
  CalcLandingZone ();
}

static inline void Dump (const TState& state)
{
  std::cerr << "FillPoints ({";
  for (int i = 0; i < NumPoints; ++i) {
    auto pt = Points[i];
    std::cerr << (i ? ", " : "") << "{" << pt.x << "," << pt.y << "}";
  }
  std::cerr << "});" << std::endl;
  std::cerr << "state = {" << state.x << ", " << state.y << ", " << state.vx << ", "
            << state.vy << ", " << state.angle << ", " << state.power << ", "
            << state.fuel << "}; // x,y,vx,vy,angle,power,fuel" << std::endl;
}
} // namespace Surface

static inline void InitTables ()
{
  for (int a = -90; a <= 90; ++a) {
    float r = (float)a * 3.1415926 / 180;
    SinTable[a + 90] = std::sin (r);
    CosTable[a + 90] = std::cos (r);
  }
}

int main (int argc, char**)
{
  InitTables ();
  if (argc == 2) {
    RunTest ();
  } else {
    RunCg ();
  }
  return 0;
}

static inline std::chrono::steady_clock::time_point Now ()
{
  return std::chrono::steady_clock::now ();
}

static inline std::chrono::steady_clock::duration Ms (int ms)
{
  return std::chrono::milliseconds (ms);
}
