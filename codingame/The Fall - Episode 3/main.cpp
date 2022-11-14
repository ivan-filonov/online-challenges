#include <cstdarg>
#include <ios>
#pragma GCC optimize("O3,inline,omit-frame-pointer,unroll-loops", \
  "unsafe-math-optimizations", "no-trapping-math")
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,mmx,avx,avx2,popcnt,rdrnd,abm,bmi2,fma")

#include <algorithm>
#include <array>
#include <bits/stdc++.h>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <immintrin.h>
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

//{{BEGIN STABLE
enum Pos { pos_top = 0, pos_left, pos_right };

static int                convert_pos (const std::string& s);
static const std::string& convert_pos (int s);
static int                pos_mask (int pos); //<bits_from_*

enum Bits {
  bit_h = 1,
  bit_v = 2,
  bit_l2b = 4,
  bit_r2b = 8,
  bit_t2l = 16,
  bit_t2r = 32,

  bits_from_left = bit_h | bit_l2b,
  bits_from_right = bit_h | bit_r2b,
  bits_from_top = bit_v | bit_t2l | bit_t2r
};

static int rotate_cell_left (int cell);
static int rotate_cell_right (int cell);
static int cell_movement_mask (int cell);
//}}END STABLE

class Entities
{
public:
  void read (std::istream& in);
};

void Entities::read (std::istream& in)
{
  int x;
  int y;

  std::string spos;

  in >> x >> y >> spos;
  in.ignore ();
  std::cerr << "I x=" << x << " y=" << y << " pos=" << spos << "\n";

  int nr;
  in >> nr;
  in.ignore ();

  for (int i = 0; i < nr; ++i) {
    in >> x >> y >> spos;
    in.ignore ();
    std::cerr << "R#" << i << " x=" << x << " y=" << y << " pos=" << spos << "\n";
  }
  std::cerr << std::flush;
}

class Board
{
public:
  void read (std::istream& in);
};

void Board::read (std::istream& in)
{
  int w;
  int h;
  int e;

  in >> w >> h;
  in.ignore ();
  std::cerr << "board width=" << w << " height=" << h << "\n";

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      int cell;
      in >> cell;
      std::cerr << (x ? " " : "") << cell;
    }
    in.ignore ();
    std::cerr << "\n";
  }

  in >> e;
  in.ignore ();
  std::cerr << "exit x=" << e << std::endl;
}

class Solver
{
public:
  void init (std::istream& in);
  void step (std::istream& in);
};

void Solver::init (std::istream& in)
{
  Board board;
  board.read (in);
}

void Solver::step (std::istream& in)
{
  Entities entities;
  entities.read (in);
  std::cout << "WAIT" << std::endl;
}

static const std::string& test_input ();

int main ()
{
#ifndef TEST
  auto& in = std::cin;
#else
  std::cerr << "TEST\n";

  auto       in = std::istringstream{test_input ()};
#endif

#ifdef TEST
  const auto max_step = 2;
#else
  const auto max_step = 420;
#endif

  Solver solver;
  solver.init (in);

  for (int step = 0; step < max_step; ++step) {
    solver.step (in);
  }
}

//{{BEGIN STABLE
static const std::string test_input_string = R"(11 10
0 3 0 0 -3 0 -3 0 -7 2 -2
0 11 3 13 3 0 -3 0 3 0 0
0 12 2 12 -11 -13 -3 0 -7 2 -2
0 3 0 0 -12 -10 -3 0 3 0 0
0 2 0 0 -11 -8 -10 0 -7 2 -2
0 11 3 -1 -2 -10 0 0 3 0 0
0 0 0 2 0 0 0 0 -7 -2 -2
0 0 0 1 -2 -2 -2 -2 -10 0 0
0 12 3 12 0 0 0 0 0 0 0
0 -3 0 0 0 0 0 0 0 0 0
1
1 0 TOP
0
1 1 TOP
2
8 0 TOP
4 0 TOP
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

static constexpr int left_rotation[14] = {0, 1, 3, 2, 5, 4, 9, 6, 7, 8, 13, 10, 11, 12};

static int rotate_cell_left (int cell)
{
  return left_rotation[cell];
}

static constexpr int right_rotation[14] = {0, 1, 3, 2, 5, 4, 7, 8, 9, 6, 11, 12, 13, 10};

static int rotate_cell_right (int cell)
{
  return right_rotation[cell];
}

static constexpr int movement_masks[14] = {0, bit_l2b | bit_v | bit_r2b, bit_h,
  bit_v, bit_t2l | bit_r2b, bit_t2r | bit_l2b, bit_h, bit_v | bit_r2b,
  bit_l2b | bit_r2b, bit_l2b | bit_v, bit_t2l, bit_t2r, bit_r2b, bit_l2b};

static int cell_movement_mask (int cell)
{
  return movement_masks[cell];
}
//}}END STABLE
