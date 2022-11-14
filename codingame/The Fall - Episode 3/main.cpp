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

class Position
{
public:
  Position (int x, int y, int pos);
  Position ();

  int x () const;
  int y () const;
  int pos () const;

  void addx (int value);
  void addy (int value);
  void pos (int value);

  int hash () const;

private:
  std::array<int8_t, 3> data;
};

enum class MoveResult { NO_MOVE, BAD_MOVE, OK };

class Entities
{
public:
  void read (std::istream& in);

  int  num_rocks () const;
  void num_rocks (int value);

  void set (int index, int x, int y, int pos);
  void set (int index, Position pos);

  Position get (int index) const;

private:
  int num_rocks_;

  std::array<Position, 11> data;
};

class Board
{
public:
  void read (std::istream& in);

  void set (int x, int y, int cell);
  int  get (int x, int y) const;

  MoveResult                 move (Position from, Position& to) const;
  std::array<MoveResult, 11> move (const Entities& from, Entities& to) const;

  int width () const;
  int height () const;
  int exit_x () const;

private:
  int width_;
  int height_;
  int exit_x_;

  std::array<std::array<int8_t, 20>, 20> grid;
};

class Solver
{
public:
  void init (std::istream& in);
  void step (std::istream& in);

  Board board;
};

void Solver::init (std::istream& in)
{
  board.read (in);
}

void Solver::step (std::istream& in)
{
  Entities entities;
  entities.read (in);
  Entities e2;
  auto     mask = board.move (entities, e2);
  for (int i = 0; i <= e2.num_rocks (); ++i) {
    auto pos = e2.get (i);
    std::cerr << "moved #" << i << " ok=" << std::boolalpha
              << (mask[i] == MoveResult::OK) << " x=" << pos.x ()
              << " y=" << pos.y () << " pos=" << convert_pos (pos.pos ()) << "\n";
  }
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

static int rotate_left (int cell)
{
  return left_rotation[cell];
}

static constexpr int right_rotation[14] = {0, 1, 3, 2, 5, 4, 7, 8, 9, 6, 11, 12, 13, 10};

static int rotate_right (int cell)
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

void Board::set (int x, int y, int cell)
{
  grid[y][x] = cell;
}

int Board::get (int x, int y) const
{
  return grid[y][x];
}

void Board::read (std::istream& in)
{
  in >> width_ >> height_;
  in.ignore ();
  std::cerr << "board width=" << width_ << " height=" << height_ << "\n";
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      int cell;
      in >> cell;
      set (x, y, cell);
      std::cerr << (x ? " " : "") << cell;
    }
    in.ignore ();
    std::cerr << "\n";
  }
  in >> exit_x_;
  in.ignore ();
  std::cerr << "exit x=" << exit_x_ << std::endl;
}

int Entities::num_rocks () const
{
  return num_rocks_;
}

void Entities::num_rocks (int value)
{
  num_rocks_ = value;
}

void Entities::set (int index, int x, int y, int pos)
{
  data[index] = Position{x, y, pos};
}

void Entities::read (std::istream& in)
{
  int x;
  int y;

  std::string pos_str;

  in >> x >> y >> pos_str;
  in.ignore ();
  std::cerr << "I x=" << x << " y=" << y << " pos=" << pos_str << "\n";
  set (0, x, y, convert_pos (pos_str));

  int r;
  in >> r;
  in.ignore ();
  num_rocks (r);

  for (int i = 0; i < r; ++i) {
    in >> x >> y >> pos_str;
    in.ignore ();
    std::cerr << "R" << i << " x=" << x << " y=" << y << " pos=" << pos_str << "\n";
    set (i + 1, x, y, convert_pos (pos_str));
  }
}

Position::Position (int x, int y, int pos)
{
  data[0] = x;
  data[1] = y;
  data[2] = pos;
}

Position::Position ()
: Position (-1, -1, -1)
{
}

int Position::x () const
{
  return data[0];
}

int Position::y () const
{
  return data[1];
}

int Position::pos () const
{
  return data[2];
}

void Entities::set (int index, Position pos)
{
  data[index] = pos;
}

Position Entities::get (int index) const
{
  return data[index];
}

void Position::addx (int value)
{
  data[0] += value;
}

void Position::addy (int value)
{
  data[1] += value;
}

void Position::pos (int value)
{
  data[2] = value;
}

int Board::width () const
{
  return width_;
}

int Board::height () const
{
  return height_;
}

int Board::exit_x () const
{
  return exit_x_;
}

MoveResult Board::move (Position from, Position& to) const
{
  to = from;
  const auto dir_mask = pos_mask (from.pos ());
  const auto cell_mask = cell_movement_mask (get (from.x (), from.y ()));
  if ((dir_mask & cell_mask) == 0) {
    return MoveResult::NO_MOVE;
  }
  int new_pos = from.pos ();
  switch (from.pos ()) {
  case pos_top:
    if (cell_mask & bit_v) {
      // pass
    } else if (cell_mask & bit_t2l) {
      new_pos = pos_right;
    } else if (cell_mask & bit_t2r) {
      new_pos = pos_left;
    }
    break;
  case pos_left:
    if (cell_mask & bit_h) {
      // pass
    } else if (cell_mask & bit_l2b) {
      new_pos = pos_top;
    }
    break;
  case pos_right:
    if (cell_mask & bit_h) {
      // pass
    } else if (cell_mask & bit_r2b) {
      new_pos = pos_top;
    }
    break;
  default:
    throw std::runtime_error{"invalid pos value " + std::to_string (from.pos ())};
  }
  to.pos (new_pos);
  bool move_is_valid = false;
  switch (new_pos) {
  case pos_top:
    to.addy (1);
    move_is_valid = to.y () < height () || to.x () == exit_x ();
    break;
  case pos_left:
    to.addx (1);
    move_is_valid = to.x () < width ();
    break;
  case pos_right:
    to.addx (-1);
    move_is_valid = to.x () >= 0;
    break;
  default:
    throw std::runtime_error{"invalid new_pos value " + std::to_string (new_pos)};
  }
  if (move_is_valid) {
    const auto target_cell_mask = cell_movement_mask (get (to.x (), to.y ()));
    const auto new_pos_bits = pos_mask (new_pos);
    move_is_valid = (target_cell_mask & new_pos_bits) != 0;
  }
  return move_is_valid ? MoveResult::OK : MoveResult::BAD_MOVE;
}

int Position::hash () const
{
  return 20 * y () + x ();
}

std::array<MoveResult, 11> Board::move (const Entities& from, Entities& to) const
{
  std::array<MoveResult, 11> result;
  result.fill (MoveResult::NO_MOVE);
  std::unordered_map<int, std::vector<int>> collisions;
  to.num_rocks (from.num_rocks ());
  for (int i = 0; i <= from.num_rocks (); ++i) {
    Position new_pos;
    result[i] = move (from.get (i), new_pos);
    if (result[i] != MoveResult::NO_MOVE) {
      to.set (i, new_pos);
    }
    if (result[i] == MoveResult::OK) {
      collisions[new_pos.hash ()].push_back (i);
    }
  }
  for (const auto& [_, indices] : collisions) {
    if (indices.size () > 1) {
      for (int i : indices) {
        result[i] = MoveResult::BAD_MOVE;
      }
    }
  }
  return result;
}
