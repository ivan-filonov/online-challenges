#pragma GCC optimize("O3", "inline", "omit-frame-pointer")
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

static const int GENE_SIZE = 50;
static const int POPULATION_SIZE = 160;
static const int MUTATIONS_COUNT = GENE_SIZE / 10;
static const int SURVIVORS_COUNT = POPULATION_SIZE / 8;
static const int RANDOM_COUNT = POPULATION_SIZE / 8;

struct Ent2f {
  float x;
  float y;

  Ent2f(float _x, float _y) : x(_x), y(_y) {}

  Ent2f operator-(const Ent2f &other) const {
    return {x - other.x, y - other.y};
  }
  Ent2f operator+(const Ent2f &other) const {
    return {x + other.x, y + other.y};
  }

  float dist2() const { return x * x + y * y; };

  Ent2f moveto(const Ent2f &target, float speed) const {
    auto dir = target - *this;
    auto dir_dist2 = dir.dist2();
    if (dir_dist2 <= speed * speed) {
      return target;
    }
    auto dir_dist = std::sqrt(dir_dist2);
    float scale = speed / dir_dist;
    dir.x = std::floor(scale * dir.x);
    dir.y = std::floor(scale * dir.y);
    return (*this) + dir;
  }

  friend std::ostream &operator<<(std::ostream &stream, const Ent2f &e) {
    stream << e.x << " " << e.y;
    return stream;
  }
};

struct State {
  std::vector<Ent2f> humans;
  std::vector<Ent2f> zombies;
  std::vector<Ent2f> zombiesNext;
  Ent2f playerMove;

  State() : playerMove(0, 0) { humans.push_back(playerMove); }

  void movePlayer(float x, float y) { playerMove = {x, y}; }

  void setPlayerPos(float x, float y) {
    humans[0].x = x;
    humans[0].y = y;
  }

  void addHuman(float x, float y) { humans.emplace_back(x, y); }

  void addZombie(float x, float y) {
    zombies.emplace_back(x, y);
    zombiesNext.push_back(zombies.back());
  }

  Ent2f getPlayerPos() const { return humans[0]; }

  void updateZombiesNext() {
    for (size_t i = 0; i < zombies.size(); ++i) {
      auto &z = zombies[i];
      auto &zn = zombiesNext[i];
      Ent2f target = humans[0];
      float dist = (target - z).dist2();
      for (size_t j = 1; j < humans.size(); ++j) {
        auto h = humans[j];
        auto hd = (h - z).dist2();
        if (hd < dist) {
          target = h;
          dist = hd;
        }
      }
      zn = z.moveto(target, 400);
    }
  }

  float score = 0;

  static int fib(int n) {
    if (n == 1) {
      return 1;
    }
    int a = 1;
    if (n == 2) {
      return 2;
    }
    int b = 2;
    for (int i = 2; i < n; ++i) {
      int c = a + b;
      a = b;
      b = c;
    }
    return b;
  }

  void killZombies(float dist) {
    const auto player = humans[0];
    const auto dist2 = dist * dist;
    int zkilled = 0;
    for (auto iter = zombies.begin(); iter != zombies.end();) {
      if ((player - *iter).dist2() <= dist2) {
        iter = zombies.erase(iter);
        zkilled += 1;
      } else {
        ++iter;
      }
    }
    if (zkilled) {
      score += 10.0 * (humans.size() - 1) * fib(zkilled);
    }
  }

  void killHumans() {
    for (auto z : zombies) {
      for (auto iter = humans.begin() + 1; iter != humans.end();) {
        if ((z - *iter).dist2() < 1) {
          iter = humans.erase(iter);
        } else {
          ++iter;
        }
      }
    }
  }

  void update() {
    updateZombiesNext();
    std::swap(zombies, zombiesNext);

    humans[0] = humans[0].moveto(playerMove, 1000);

    killZombies(2000);
    killHumans();

    if(false)
    {
      std::cerr << "A " << humans[0] << "\n";
      for (size_t i = 1; i < humans.size(); ++i) {
        std::cerr << "H " << humans[i] << "\n";
      }
      for (auto z : zombies) {
        std::cerr << "Z " << z << "\n";
      }
      std::cerr << std::endl;
    }
  }

  bool finished() const { return humans.size() == 1 || zombies.empty(); }
};

static std::mt19937 rd{static_cast<std::mt19937>(std::random_device{}())};

static int AliveCount = 0;
static int Sorted[POPULATION_SIZE];
static float GenesX[GENE_SIZE * POPULATION_SIZE];
static float GenesY[GENE_SIZE * POPULATION_SIZE];
static float Fitness[POPULATION_SIZE];

static inline float randf(float min, float max) {
  return std::uniform_real_distribution<float>(min, max)(rd);
}

static inline int randi(int min, int max) {
  return std::uniform_int_distribution<int>(min, max)(rd);
}

static inline int AllocGenome() {
  const int result = Sorted[AliveCount];
  Fitness[result] = -1e9;
  AliveCount += 1;
  return result;
}

static float *X(int idx) { return GenesX + idx * GENE_SIZE; }

static float *Y(int idx) { return GenesY + idx * GENE_SIZE; }

static void ShiftGenes(int idx) {
  float *p = X(idx);
  std::copy(p + 1, p + GENE_SIZE, p);
  p = Y(idx);
  std::copy(p + 1, p + GENE_SIZE, p);
}

static void Blend(float *a, float *b, float ka, float *dst) {
  for (int i = 0; i < GENE_SIZE; ++i) {
    dst[i] = a[i] * ka + b[i] * (1.f - ka);
  }
}

static void Mutate(int idx) {
  float *x = X(idx);
  float *y = Y(idx);
  for (int i = 0; i < MUTATIONS_COUNT; ++i) {
    int o = randi(0, GENE_SIZE);
    x[o] = randf(0, 16000);
    y[o] = randf(0, 9000);
  }
}

static void AddBreeds() {
  const int a = Sorted[randi(0, SURVIVORS_COUNT - 1)];
  const int b = Sorted[randi(0, SURVIVORS_COUNT - 1)];
  const int na = AllocGenome();
  const int nb = AllocGenome();
  const float wlim = 0.2;
  const float w = randf(wlim, 1. - wlim);
  Blend(X(a), X(b), w, X(na));
  Blend(Y(a), Y(b), w, Y(na));
  Mutate(na);

  Blend(X(a), X(b), 1. - w, X(nb));
  Blend(Y(a), Y(b), 1. - w, Y(nb));
  Mutate(nb);
}

static void AddRandom(const State &state) {
  const int idx = AllocGenome();
  float *x = X(idx);
  float *y = Y(idx);
  for (int i = 0; i < GENE_SIZE; ++i) {
    if (i > 0 && randf(0, 1) > .5) {
      int z = randi(0, state.zombies.size() - 1);
      x[i] = state.zombies[z].x;
      y[i] = state.zombies[z].y;
    } else if (randf(0, 1) > .95) {
      int p = randi(1, state.humans.size() - 1);
      x[i] = state.humans[p].x;
      y[i] = state.humans[p].y;
    } else {
      x[i] = randf(0, 16000);
      y[i] = randf(0, 9000);
    }
  }
}

static void EvalPopulation(const State &state) {
  for (int i = 0; i < AliveCount; ++i) {
    int idx = Sorted[i];
    auto copied = state;
    const float *x = X(idx);
    const float *y = Y(idx);
    for (int step = 0; step < GENE_SIZE && !copied.finished(); ++step) {
      copied.movePlayer(x[step], y[step]);
      copied.update();
    }
    Fitness[idx] = copied.humans.size() > 1 ? copied.score : 0;
  }
}

static inline int CompareGenes(int a, int b) { return Fitness[a] > Fitness[b]; }

static inline void SolverInit() {
  AliveCount = 0;
  std::iota (Sorted, Sorted + POPULATION_SIZE, 0);
}

static void SolverStep(const State&state,int timeLimitMs) {
  auto deadline =
      std::chrono::steady_clock::now() + std::chrono::milliseconds(timeLimitMs);
  if(AliveCount > SURVIVORS_COUNT) {
    AliveCount = SURVIVORS_COUNT;
  }
  for(int i = 0; i < AliveCount; ++i) {
    ShiftGenes(Sorted[i]);
  }
  int gen_cnt = 0;
  while(std::chrono::steady_clock::now() < deadline) {
    if(AliveCount >= SURVIVORS_COUNT) {
      while(AliveCount < POPULATION_SIZE - RANDOM_COUNT) {
        AddBreeds();
      }
    }
    while(AliveCount < POPULATION_SIZE) {
      AddRandom(state);
    }
    EvalPopulation(state);
    std::sort (Sorted, Sorted + POPULATION_SIZE, CompareGenes);
    gen_cnt += 1;
  }
  std::cerr << "gen_cnt = " << gen_cnt
            << ", best score = " << Fitness[Sorted[0]] << "\n";
}

static int nearestIndex(Ent2f e, const std::vector<Ent2f>& v) {
  float d2 = 1e9;
  int res = -1;
  for(int i = 1; i < v.size(); ++i) {
    float di2 = (e-v[i]).dist2();
    if(di2 < d2) {
      d2 = di2;
      res = i;
    }
  }
  return res;
}

int main ()
{
  SolverInit();

  // game loop
  while (1) {
    State state;
    int x;
    int y;
    std::cin >> x >> y;
    std::cin.ignore ();
    state.setPlayerPos(x, y);
    int humanCount;
    std::cin >> humanCount;
    std::cin.ignore ();
    int humanX;
    int humanY;
    for (int i = 0; i < humanCount; i++) {
      int humanId;
      std::cin >> humanId >> humanX >> humanY;
      std::cin.ignore ();
      state.addHuman(humanX, humanY);
    }
    int zombieCount;
    std::cin >> zombieCount;
    std::cin.ignore ();

    int zombieX;
    int zombieY;
    for (int i = 0; i < zombieCount; i++) {
      int zombieId;
      int zombieXNext;
      int zombieYNext;
      std::cin >> zombieId >> zombieX >> zombieY >> zombieXNext >> zombieYNext;
      std::cin.ignore ();
      state.addZombie(zombieX, zombieY);
    }

    SolverStep(state, 90);

    int cmd_x = int(X(Sorted[0])[0]);
    int cmd_y = int(Y(Sorted[0])[0]);
    if(Fitness[Sorted[0]] < 10) {
      int i = nearestIndex({x,y}, state.humans);
      cmd_x = state.humans[i].x;
      cmd_y = state.humans[i].y;
    }

    std::cout << cmd_x << " " << cmd_y << std::endl; // Your destination coordinates
  }
}
