#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <istream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

static constexpr int left_rotation[14] = {0, 1, 3, 2, 5, 4, 9, 6, 7, 8, 13, 10, 11, 12};
static constexpr int right_rotation[14] = {0, 1, 3, 2, 5, 4, 7, 8, 9, 6, 11, 12, 13, 10};

static constexpr int pos_top = 0;
static constexpr int pos_left = 1;
static constexpr int pos_right = 2;

static int                convert_pos (const std::string& s);
static const std::string& convert_pos (int s);

static constexpr int bit_h = 1;
static constexpr int bit_v = 2;
static constexpr int bit_l2b = 4;
static constexpr int bit_r2b = 8;
static constexpr int bit_t2l = 16;
static constexpr int bit_t2r = 32;

static constexpr int bits_from_left = bit_h | bit_l2b;
static constexpr int bits_from_right = bit_h | bit_r2b;
static constexpr int bits_from_top = bit_v | bit_t2l | bit_t2r;

static constexpr int movement_masks[14] = {0, bit_l2b | bit_v | bit_r2b, bit_h,
  bit_v, bit_t2l | bit_r2b, bit_t2r | bit_l2b, bit_h, bit_v | bit_r2b,
  bit_l2b | bit_r2b, bit_l2b | bit_v, bit_t2l, bit_t2r, bit_r2b, bit_l2b};

static const std::string& test_input ();

struct Cmd {
  int rotation;
  int x;
  int y;

  std::string to_string ();
};

struct Board {
  int width;
  int height;
  int exit_x;

  std::array<int8_t, 20 * 20> board;

  void read_from (std::istream& in);
  bool step (int x, int y, int pos, int* out_x, int* out_y, int* out_pos);
  void apply (const Cmd& cmd);
};

struct State {
  std::array<int8_t, 11> x;
  std::array<int8_t, 11> y;
  std::array<int8_t, 11> pos;

  int num_rocks;

  void read_from (std::istream& in);
};

enum class SimResult { FORK, WIN, FAIL };
std::string to_string (SimResult sr);

struct Solver {
  Board board;

  void init (const Board& board);

  std::string run (const State& state);
};

std::string Solver::run (const State& state)
{
  return "WAIT";
}

int main ()
{
#ifndef TEST
  auto& in = std::cin;
  auto& out = std::cout;
#else
  std::cerr << "TEST\n";
  auto  in = std::istringstream{test_input ()};
  auto& out = std::cout;
#endif

  Board board;
  board.read_from (in);
  const auto max_step = 4; //(board.width + 1) * board.height

  Solver solver;
  solver.init (board);

  for (int step = 0; step < max_step; ++step) {
    State state;
    state.read_from (in);
    out << solver.run (state) << std::endl;
  }
}

static const std::string test_input_string = R"(14 8
0 0 3 0 0 0 -3 0 0 0 -11 -2 -13 0
0 0 -7 2 4 3 5 7 3 2 11 0 -3 0
-12 -2 4 2 10 0 0 2 11 3 5 2 -9 0
-3 0 0 0 3 0 0 3 2 0 0 0 3 0
-3 0 0 13 2 3 2 12 9 2 3 3 10 0
-3 2 3 3 4 3 3 2 2 0 0 0 0 0
-11 -2 -2 -2 -6 -2 -8 3 5 2 2 11 0 0
0 0 0 0 0 0 3 0 0 0 0 2 0 0
6
6 0 TOP
1
2 0 TOP
)";

static const std::string& test_input ()
{
  return test_input_string;
}

static const std::array<std::string, 3> pos_names = {"TOP", "LEFT", "RIGHT"};

static const std::string& convert_pos (int pos)
{
  return pos_names.at (pos);
}

static int convert_pos (const std::string& s)
{
  if (s == "TOP") {
    return pos_top;
  } else if (s == "LEFT") {
    return pos_left;
  } else {
    return pos_right;
  }
}

void Board::read_from (std::istream& in)
{
  in >> width >> height;
  std::cerr << "width=" << width << " height=" << height << std::endl;
  in.ignore ();
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int cell;
      in >> cell;
      board[20 * y + x] = cell;
      std::cerr << (x > 0 ? " " : "") << cell;
    }
    in.ignore ();
    std::cerr << std::endl;
  }
  in >> exit_x;
  in.ignore ();
  std::cerr << "exit_x=" << exit_x << std::endl;
}

void State::read_from (std::istream& in)
{
  int index = 0;
  int ix = -1;
  int iy = -1;

  std::string pos_str;

  in >> ix >> iy >> pos_str;
  in.ignore ();
  x[index] = ix;
  y[index] = iy;
  pos[index] = convert_pos (pos_str);
  std::cerr << ix << " " << iy << " " << pos_str << std::endl;

  in >> num_rocks;
  in.ignore ();
  std::cerr << num_rocks << std::endl;

  for (index = 1; index <= num_rocks; ++index) {
    in >> ix >> iy >> pos_str;
    in.ignore ();
    x[index] = ix;
    y[index] = iy;
    pos[index] = convert_pos (pos_str);
    std::cerr << ix << " " << iy << " " << pos_str << std::endl;
  }
}

void Solver::init (const Board& initial_board)
{
  board = initial_board;
}

bool Board::step (int x, int y, int pos, int* out_x, int* out_y, int* out_pos)
{
  const auto index = y * 20 + x;
  const auto cell_value = board[index];
  const auto cell_bits = movement_masks[std::abs (cell_value)];
  switch (pos) {
  case pos_top:
    if (cell_bits & bit_v) {
      *out_pos = pos_top;
    } else if (cell_bits & bit_t2l) {
      *out_pos = pos_right;
    } else if (cell_bits & bit_t2r) {
      *out_pos = pos_left;
    } else {
      return false;
    }
    break;
  case pos_left:
    if (cell_bits & bit_h) {
      *out_pos = pos_left;
    } else if (cell_bits & bit_l2b) {
      *out_pos = pos_top;
    } else {
      return false;
    }
    break;
  case pos_right:
    if (cell_bits & bit_h) {
      *out_pos = pos_right;
    } else if (cell_bits & bit_r2b) {
      *out_pos = pos_top;
    } else {
      return false;
    }
    break;
  default: return false;
  }
  if (*out_pos == pos_top) {
    *out_x = x;
    *out_y = y + 1;
    return *out_y < height || *out_x == exit_x;
  } else if (*out_pos == pos_left) {
    *out_x = x + 1;
    *out_y = y;
    return *out_x < width;
  } else if (*out_pos == pos_right) {
    *out_x = x - 1;
    *out_y = y;
    return *out_x >= 0;
  }
  return false;
}

void Board::apply (const Cmd& cmd)
{
  if (cmd.rotation == 0) {
    return;
  }
  const int index = cmd.y * 20 + cmd.x;

  auto& ref = board[index];
  if (ref < 2 || ref > 13) {
    throw std::runtime_error ("invalid cell rotation");
  }
  if (cmd.rotation == -1) {
    ref = left_rotation[ref];
    return;
  }
  if (cmd.rotation == 1) {
    ref = right_rotation[ref];
    return;
  }
  throw std::runtime_error ("malformed command");
}

std::string Cmd::to_string ()
{
  if (rotation == 0) {
    return "WAIT";
  }
  std::stringstream ss;
  ss << x << " " << y;
  if (rotation < 0) {
    ss << " LEFT";
  } else {
    ss << " RIGHT";
  }
  return ss.str ();
}

std::string to_string (SimResult sr)
{
  switch (sr) {
  case SimResult::FORK: return "FORK";
  case SimResult::FAIL: return "FAIL";
  case SimResult::WIN: return "WIN";
  }
}
