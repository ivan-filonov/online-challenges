#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <istream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
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

static int pos_mask (int pos);

struct Board {
  void read_from (std::istream& in);
  int  get (int x, int y) const;
  void put (int x, int y, int cell);
  int  width () const;
  int  height () const;
  int  exit_x () const;

private:
  std::array<int8_t, 20 * 20 + 3> data;

  void size (int width, int height);
  void exit_x (int value);
};

struct Entities {
  void                      pos (int index, int x, int y, int pos);
  std::tuple<int, int, int> pos (int index) const;
  int                       num_rocks () const;
  void                      read_from (std::istream& in);

  std::string     to_string () const;
  static Entities parse (const std::string& s);

private:
  std::array<int8_t, 11 * 3 + 1> data;

  void num_rocks (int num);
};

struct Solver {
  void        init (const Board& board);
  std::string run (const Entities& state);
};

void Solver::init (const Board& initial_board)
{
}

std::string Solver::run (const Entities& initial_state)
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
  auto       in = std::istringstream{test_input ()};
  auto&      out = std::cout;
#endif

  Board board;
  board.read_from (in);

#ifdef TEST
  const auto max_step = 2;
#else
  const auto max_step = 400;
#endif

  Solver solver;
  solver.init (board);

  for (int step = 0; step < max_step; ++step) {
    Entities state;
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

static int pos_mask (int pos)
{
  switch (pos) {
  case pos_top: return bits_from_top;
  case pos_left: return bits_from_left;
  case pos_right: return bits_from_right;
  default: return 0;
  }
}

void Entities::pos (int index, int x, int y, int pos)
{
  data[index] = x;
  data[index + 11] = y;
  data[index + 22] = pos;
}

void Entities::num_rocks (int num)
{
  if (num < 0 || num > 10) {
    throw std::runtime_error{"invalid number of rocks: " + std::to_string (num)};
  }
  data[33] = num;
}

std::tuple<int, int, int> Entities::pos (int index) const
{
  return {data[index], data[index + 11], data[index + 22]};
}

int Entities::num_rocks () const
{
  return data[33];
}

void Entities::read_from (std::istream& in)
{
  int x = 0;
  int y = 0;

  std::string pos_str{};

  in >> x >> y >> pos_str;
  std::cerr << "I x=" << x << " y=" << y << " pos=" << pos_str << std::endl;
  in.ignore ();
  pos (0, x, y, convert_pos (pos_str));

  int num_rocks;
  in >> num_rocks;
  in.ignore ();
  this->num_rocks (num_rocks);

  for (int i = 0; i < num_rocks; ++i) {
    in >> x >> y >> pos_str;
    std::cerr << "R" << (i + 1) << " x=" << x << " y=" << y
              << " pos=" << pos_str << std::endl;
    in.ignore ();
    pos (i + 1, x, y, convert_pos (pos_str));
  }
}

int Board::width () const
{
  return data[400 + 0];
}

int Board::height () const
{
  return data[400 + 1];
}

int Board::exit_x () const
{
  return data[400 + 2];
}

void Board::exit_x (int value)
{
  data[400 + 2] = value;
}

void Board::size (int width, int height)
{
  data[400 + 0] = width;
  data[400 + 1] = height;
}

int Board::get (int x, int y) const
{
  return data[y * 20 + x];
}

void Board::put (int x, int y, int cell)
{
  data[y * 20 + x] = cell;
}

void Board::read_from (std::istream& in)
{
  int width, height, exit_x;
  in >> width >> height;
  std::cerr << "width=" << width << " height=" << height << std::endl;
  in.ignore ();
  size (width, height);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int cell;
      in >> cell;
      std::cerr << (x > 0 ? " " : "") << cell;
      put (x, y, cell);
    }
    in.ignore ();
  }

  in >> exit_x;
  std::cerr << "exit x=" << exit_x << std::endl;
  in.ignore ();
  this->exit_x (exit_x);
}

std::string Entities::to_string () const
{
  std::string s;
  s.resize (2 + 2 * num_rocks (), ' ');
  for (int i = 0; i <= num_rocks (); ++i) {
    auto [x, y, pos] = this->pos (i);
    s[i * 2 + 0] = (((pos & 1) != 0) ? 'A' : 'a') + x;
    s[i * 2 + 1] = (((pos & 2) != 0) ? 'A' : 'a') + y;
  }
  return s;
}

Entities Entities::parse (const std::string& s)
{
  Entities ent;
  ent.num_rocks (s.length () / 2 - 1);
  for (int i = 0; i <= ent.num_rocks (); ++i) {
    int x, y, p;
    x = s[i * 2 + 0];
    y = s[i * 2 + 1];
    p = 0;
    if (std::isupper (x)) {
      p += 1;
      x -= 'A';
    } else {
      x -= 'a';
    }
    if (std::isupper (y)) {
      p += 2;
      y -= 'A';
    } else {
      y -= 'a';
    }
    ent.pos (i, x, y, p);
  }
  return ent;
}
