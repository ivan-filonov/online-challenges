#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class Dir { Up, Left, Down, Right };

std::unordered_map<std::string, Dir> str2dir{
    {"D", Dir::Down},
    {"L", Dir::Left},
    {"R", Dir::Right},
    {"U", Dir::Up},
};
std::unordered_map<char, Dir> char2dir{
    {'D', Dir::Down},
    {'L', Dir::Left},
    {'R', Dir::Right},
    {'U', Dir::Up},
};
static constexpr char dir2char[] = "ULDR";

static constexpr int FIELD_HEIGHT = 10;
static constexpr int FIELD_WIDTH = 19;

struct Robot {
  int x;
  int y;
  Dir direction;
  std::vector<bool> history;

  int pos_hash() const;

  void ensure_history();
  void update();
  void change_direction(Dir dir);
  bool check() const;

  Robot(int x_, int y_, Dir dir_) : x(x_), y(y_), direction(dir_) {}
};

int Robot::pos_hash() const {
  return (x + y * FIELD_WIDTH) * 4 + static_cast<int>(direction);
}

void Robot::ensure_history() {
  if (history.empty()) {
    history.resize(FIELD_WIDTH * FIELD_HEIGHT * 4, false);
  }
}

void Robot::update() {
  ensure_history();
  history.at(pos_hash()) = true;
  switch (direction) {
  case Dir::Down:
    y = (y + 1) % FIELD_HEIGHT;
    break;
  case Dir::Left:
    x = (x + FIELD_WIDTH - 1) % FIELD_WIDTH;
    break;
  case Dir::Right:
    x = (x + 1) % FIELD_WIDTH;
    break;
  case Dir::Up:
    y = (y + FIELD_HEIGHT - 1) % FIELD_HEIGHT;
    break;
  default:
    std::cerr << "will throw\n";
    throw std::logic_error{"invalid direction: " +
                           std::to_string(static_cast<int>(direction))};
  }
  if (x < 0 || y < 0 || x >= FIELD_WIDTH || y >= FIELD_HEIGHT) {
    std::cerr << "will throw\n";
    throw std::exception{};
  }
}

void Robot::change_direction(Dir dir) {
  ensure_history();
  if (dir != direction) {
    history[pos_hash()] = true;
    direction = dir;
  }
}

bool Robot::check() const { return history.empty() || !history[pos_hash()]; }

struct State {
  std::array<std::array<char, FIELD_WIDTH>, FIELD_HEIGHT> board;
  std::vector<Robot> robots;
  int score = 0;

  bool update();
  void first_update();

  void parse(std::istream &in);
  void apply_arrows(const std::string &s);

  bool putArrow(int x, int y, Dir dir);

  char get(int x, int y) const;
  void put(int x, int y, char c);

  int evaluate();
};

static std::mt19937 rd{static_cast<std::mt19937>(std::random_device{}())};

static inline float randf(float min, float max) {
  return std::uniform_real_distribution<float>(min, max)(rd);
}

static inline int randi(int min, int max) {
  return std::uniform_int_distribution<int>(min, max)(rd);
}

struct Chromosome {
  std::vector<bool> active;
  std::vector<Dir> dir;
  int score;

  Chromosome(size_t size, float prob_active);
  Chromosome(const Chromosome &ca, const Chromosome &cb, float k);
  bool operator<(const Chromosome &other) const;
};

Chromosome::Chromosome(size_t size, float prob_active) {
  active.clear();
  dir.clear();
  for (size_t i = 0; i < size; ++i) {
    active.push_back(randf(0, 1) < prob_active);
    dir.push_back(static_cast<Dir>(randi(0, 3)));
  }
}

Chromosome::Chromosome(const Chromosome &ca, const Chromosome &cb, float k) {
  active = ca.active;
  dir = ca.dir;
  for (size_t i = 0; i < active.size(); ++i) {
    bool useb = randf(0, 1) > k;
    if (useb) {
      active[i] = cb.active[i];
      dir[i] = cb.dir[i];
    }
  }
}

bool Chromosome::operator<(const Chromosome &other) const {
  return score > other.score;
}

struct GA {
  std::vector<int> gx;
  std::vector<int> gy;
  std::vector<Chromosome> chromosomes;

  int m_num_initial = 100;
  int m_num_survivors = 20;
  int m_num_breeds = 60;
  int m_num_random = 20;
  float m_prob_active = 0.1f;

  const State &state;

  void start();
  void evaluate();
  void repopulate();

  void solve(std::chrono::steady_clock::duration time_limit);
  std::string get_best() const;

  explicit GA(const State &state_) : state(state_) {
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
      for (int x = 0; x < FIELD_WIDTH; ++x) {
        if (state.get(x, y) == '.') {
          gx.push_back(x);
          gy.push_back(y);
        }
      }
    }
    std::cerr << "chromosome size will be " << gx.size() << "\n";
  }
};

void GA::start() {
  chromosomes.clear();
  for (int i = 0; i < m_num_initial; ++i) {
    chromosomes.emplace_back(Chromosome{gx.size(), m_prob_active});
  }
}

void GA::evaluate() {
  for (auto &cc : chromosomes) {
    auto state_copy = state;
    for (size_t i = 0; i < cc.active.size(); ++i) {
      if (cc.active[i]) {
        state_copy.putArrow(gx[i], gy[i], cc.dir[i]);
      }
    }
    //    std::cerr << "state copy prepared\n";
    cc.score = state_copy.evaluate();
    //    std::cerr << "score -> " << cc.score << "\n";
  }
  std::sort(chromosomes.begin(), chromosomes.end());
}

void GA::repopulate() {
  while (chromosomes.size() > m_num_survivors) {
    chromosomes.pop_back();
  }
  for (int i = 0; i < m_num_breeds / 2; ++i) {
    const auto &ca = chromosomes[randi(0, chromosomes.size() - 1)];
    const auto &cb = chromosomes[randi(0, chromosomes.size() - 1)];
    const float k = randf(0.1, 0.9);
    chromosomes.emplace_back(Chromosome{ca, cb, k});
    chromosomes.emplace_back(Chromosome{ca, cb, 1 - k});
  }
  for (int i = 0; i < m_num_random; ++i) {
    chromosomes.emplace_back(Chromosome{gx.size(), m_prob_active});
  }
}

void GA::solve(std::chrono::steady_clock::duration time_limit) {
  const auto deadline = std::chrono::steady_clock::now() + time_limit;

  start();
  evaluate();
  int best_score = chromosomes[0].score;

  int gen = 0;
  while (std::chrono::steady_clock::now() < deadline) {
    repopulate();
    evaluate();
    best_score = std::max(best_score, chromosomes[0].score);
    ++gen;
  }
  std::cerr << "gen=" << gen << ", best_score=" << best_score << "\n";
}

std::string GA::get_best() const {
  const auto &c = chromosomes.at(0);
  std::stringstream ss;
  bool flag = false;
  for (size_t i = 0; i < c.active.size(); ++i) {
    if (!c.active[i]) {
      continue;
    }
    if (flag) {
      ss << " ";
    } else {
      flag = true;
    }
    ss << gx[i] << " " << gy[i] << " " << dir2char[static_cast<int>(c.dir[i])];
  }
  return ss.str();
}

bool State::update() {
  if (robots.empty()) {
    return false;
  }
  score += robots.size();

  for (auto iter = robots.begin(); iter != robots.end();) {
    iter->update();
    char c = get(iter->x, iter->y);
    if (std::isalpha(c)) {
      iter->change_direction(char2dir.at(c));
    }

    if (!iter->check() || get(iter->x, iter->y) == '#') {
      iter = robots.erase(iter);
    } else {
      ++iter;
    }
  }

  return !robots.empty();
}

void State::first_update() {
  score = 0;
  for (auto &robot : robots) {
    const char c = get(robot.x, robot.y);
    auto dir_iter = char2dir.find(c);
    if (dir_iter != char2dir.end()) {
      robot.change_direction(dir_iter->second);
    }
  }
}

void State::parse(std::istream &in) {
  std::string line;
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    std::getline(in, line);
    std::copy(line.begin(), line.end(), board[i].begin());
  }
  int rc = 0;
  in >> rc;
  in.ignore();
  std::cerr << "num_robots = " << rc << "\n";
  for (int i = 0; i < rc; ++i) {
    int x, y;
    std::string direction;
    in >> x >> y >> direction;
    in.ignore();
    robots.emplace_back(x, y, str2dir.at(direction));
  }
}

void State::apply_arrows(const std::string &s) {
  std::stringstream ss{s};
  for (;;) {
    int x, y;
    std::string dir;
    if (!(ss >> x)) {
      break;
    }
    if (!(ss >> y)) {
      break;
    }
    if (!(ss >> dir)) {
      break;
    }
    if (str2dir.count(dir) == 0) {
      std::cerr << "will throw\n";
      throw std::logic_error{"invalid direction: " + dir};
    }
    put(x, y, dir.front());
  }
}

bool State::putArrow(int x, int y, Dir dir) {
  if (get(x, y) != '.') {
    return false;
  }
  put(x, y, dir2char[static_cast<int>(dir)]);
  return true;
}

char State::get(int x, int y) const { return board[y][x]; }

void State::put(int x, int y, char c) { board[y][x] = c; }

int State::evaluate() {
  first_update();
  //  std::cerr << __func__ << ": first update finished\n";
  int max_step = FIELD_WIDTH * FIELD_HEIGHT * 4;
  while (update()) {
    if (!--max_step) {
      std::cerr << "MAX number of steps reached\n";
      return 0;
    }
  }
  return score;
}

int main()
{
    State state;
    state.parse(std::cin);

    GA ga{state};
    ga.m_prob_active = .2;
    ga.m_num_breeds = 60;
    ga.m_num_random = 20;
    ga.m_num_survivors = 20;
    ga.m_num_initial = 500;
    ga.solve(std::chrono::milliseconds(950));
    std::cout << ga.get_best() << std::endl;
}

