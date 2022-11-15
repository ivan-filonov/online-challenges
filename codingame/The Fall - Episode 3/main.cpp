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
static int cell_movement_mask_soft (int cell);
//}}END STABLE

class Entity
{
public:
  int x () const
  {
    return data[0];
  }

  int y () const
  {
    return data[1];
  }

  int coord_hash () const
  {
    return y () * 20 + x ();
  }

  int pos () const
  {
    return data[2];
  }

  void x (int value)
  {
    data[0] = value;
  }

  void y (int value)
  {
    data[1] = value;
  }

  void pos (int value)
  {
    data[2] = value;
  }

  void addx (int value)
  {
    data[0] += value;
  }

  void addy (int value)
  {
    data[1] += value;
  }

  void set (int x, int y, int pos)
  {
    data[0] = x;
    data[1] = y;
    data[2] = pos;
  }

private:
  std::array<int8_t, 3> data;
};

enum MoveResult { MOVE_OK, MOVE_BAD, NO_MOVE };

class Entities
{
public:
  void read (std::istream& in)
  {
    int x;
    int y;

    std::string spos;

    in >> x >> y >> spos;
    in.ignore ();
    std::cerr << "I x=" << x << " y=" << y << " pos=" << spos << "\n";
    this->operator[] (0).set (x, y, convert_pos (spos));

    int nr;
    in >> nr;
    in.ignore ();
    this->num_rocks (nr);

    for (int i = 1; i <= nr; ++i) {
      in >> x >> y >> spos;
      in.ignore ();
      std::cerr << "R#" << i << " x=" << x << " y=" << y << " pos=" << spos << "\n";
      this->operator[] (i).set (x, y, convert_pos (spos));
    }
    std::cerr << std::flush;
  }

  Entity& operator[] (int index)
  {
    return data[index];
  }

  int num_rocks () const
  {
    return num_rocks_;
  }

  void num_rocks (int value)
  {
    num_rocks_ = value;
  }

  void pack (const std::array<MoveResult, 11>& mask, Entities& out)
  {
    out[0] = this->operator[] (0);
    int            dst = 1;
    for (int src = 1; src <= num_rocks (); ++src) {
      if (mask[src] != MOVE_OK) {
        continue;
      }
      out[dst] = this->operator[] (src);
      ++dst;
    }
    out.num_rocks (dst - 1);
  }

private:
  int num_rocks_;

  std::array<Entity, 11> data;
};

class Board
{
public:
  int width () const
  {
    return width_;
  }
  int height () const
  {
    return height_;
  }
  int exit_x () const
  {
    return exit_x_;
  }
  int8_t& cell (int x, int y)
  {
    return grid[y][x];
  }
  void read (std::istream& in)
  {
    in >> width_ >> height_;
    in.ignore ();
    std::cerr << "board width=" << width_ << " height=" << height_ << "\n";

    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        int cell_;

        in >> cell_;
        std::cerr << (x ? " " : "") << cell_;
        cell (x, y) = cell_;
      }
      in.ignore ();
      std::cerr << "\n";
    }

    in >> exit_x_;
    in.ignore ();
    std::cerr << "exit x=" << exit_x_ << std::endl;
  }
  MoveResult move (const Entity& from, Entity& to)
  {
    to = from;
    // cannot move in current -> no move
    // can exit to outside -> no move
    // can exit current, cannot enter next -> bad move
    // can exit, can enter -> ok
    const auto entity_bits = pos_mask (from.pos ());
    const auto cell_bits = cell_movement_mask (cell (from.x (), from.y ()));
    if (0 == (entity_bits & cell_bits)) {
      // no valid path through current
      return NO_MOVE;
    }
    int new_pos = from.pos ();
    switch (from.pos ()) {
    case pos_top:
      if (cell_bits & bit_t2l) {
        new_pos = pos_right;
      } else if (cell_bits & bit_t2r) {
        new_pos = pos_left;
      }
      break;
    case pos_left:
      if (cell_bits & bit_l2b) {
        new_pos = pos_top;
      }
      break;
    case pos_right:
      if (cell_bits & bit_r2b) {
        new_pos = pos_top;
      }
      break;
    }
    to.pos (new_pos);
    bool hard_move = false;
    bool exit_point = false;
    switch (new_pos) {
    case pos_top:
      to.addy (1);
      hard_move = to.y () < height () || to.x () == exit_x ();
      exit_point = to.y () == height () && to.x () == exit_x ();
      break;
    case pos_left:
      to.addx (1);
      hard_move = to.x () < width ();
      break;
    case pos_right:
      to.addx (-1);
      hard_move = to.x () >= 0;
      break;
    }
    if (!hard_move) {
      return NO_MOVE;
    }
    if (exit_point) {
      return MOVE_OK;
    }
    const int dst_cell = std::abs (cell (to.x (), to.y ()));
    if (cell_movement_mask (dst_cell) & pos_mask (new_pos)) {
      return MOVE_OK;
    }
    const bool soft_move = std::abs (dst_cell) >= 2;
    return soft_move ? MOVE_BAD : NO_MOVE;
  }
  std::array<MoveResult, 11> move (Entities& from, Entities& to)
  {
    std::array<MoveResult, 11> mask;
    mask.fill (NO_MOVE);
    std::unordered_map<int, std::vector<int>> collisions;
    to.num_rocks (from.num_rocks ());
    for (int i = 0; i <= from.num_rocks (); ++i) {
      mask[i] = move (from[i], to[i]);
      if (i == 0 && mask[i] == NO_MOVE) {
        return mask;
      }
      if (mask[i] == MOVE_OK) {
        collisions[to[i].coord_hash ()].push_back (i);
        collisions[from[i].coord_hash ()].push_back (i);
      }
    }
    for (const auto& [_, indices] : collisions) {
      if (indices.size () == 1) {
        continue;
      }
      for (int i : indices) {
        mask[i] = MOVE_BAD;
      }
    }
    return mask;
  }

private:
  int width_;
  int height_;
  int exit_x_;

  std::array<std::array<int8_t, 20>, 20> grid;
};


struct State {
  using Ptr = std::shared_ptr<State>;

  Board                           board;
  Entities                        entities;
  int                             cmd_budget;
  std::vector<std::array<int, 3>> cmds;

  State (const Board& board_, const Entities& entities_)
  : board (board_)
  , entities (entities_)
  {
    cmd_budget = 1;
  }

  static Ptr create (const Board& board_, const Entities& entities_)
  {
    return std::make_shared<State> (board_, entities_);
  }

  Ptr clone () const
  {
    Ptr cloned = create (board, entities);
    cloned->cmd_budget = cmd_budget;
    cloned->cmds = cmds;
    return cloned;
  }
};

class Solver
{
public:
  void               init (std::istream& in);
  void               step (std::istream& in);
  std::array<int, 3> solve (State::Ptr first_state);

private:
  Board board;
};

void Solver::init (std::istream& in)
{
  board.read (in);
}

std::array<int, 3> Solver::solve (State::Ptr first_state)
{
  std::vector<State::Ptr> bag;
  bag.push_back (first_state);
  int choices_traced = 0;
  while (!bag.empty ()) {
    auto state = bag.back ();
    bag.pop_back ();
    ++choices_traced;

    Entities e[2];

    int ei = 0;
    int cnt = 0;

    e[ei] = state->entities;
    for (;; ei = 1 - ei, cnt += 1) {
      auto* from = &e[ei];
      auto* to = &e[1 - ei];

      Entities temp;

      auto mask = state->board.move (*from, temp);
      if (mask[0] == NO_MOVE) {
        // std::cerr << "interrupting [" << choices_traced
        //           << "] as cannot move from " << (*from)[0].x () << ","
        //           << (*from)[0].y () << "," << (*from)[0].pos () << " (cell "
        //           << (int)state->board.cell ((*from)[0].x (), (*from)[0].y ()) << ")\n";
        break;
      }

      // std::cerr << "[" << choices_traced << "] testing move of 0 from "
      //           << (*from)[0].x () << "," << (*from)[0].y () << " to "
      //           << temp[0].x () << "," << temp[0].y () << " => " << mask[0] << "\n";

      if (mask[0] == MOVE_OK && temp[0].x () == state->board.exit_x ()
          && temp[0].y () == state->board.height ()) {
        std::cerr << "see victory at step " << choices_traced << "\n";
        constexpr bool log400 = false;
        if (log400) {
          for (size_t i = 0; i < state->cmds.size (); ++i) {
            std::cerr << " cmd[" << i << "] is " << state->cmds[i][0] << ", "
                      << state->cmds[i][1] << ", " << state->cmds[i][2] << "\n";
          }
        }
        if (state->cmds.empty ()) {
          return {};
        } else {
          return state->cmds.front ();
        }
      }

      for (int i = 0; i <= temp.num_rocks (); ++i) {
        if (mask[i] == NO_MOVE) {
          continue;
        }
        auto ent = temp[i];
        int  cell = state->board.cell (ent.x (), ent.y ());
        if (cell >= 2 && state->cmd_budget + cnt >= 1) {
          // std::cerr << "will fork " << i << " at " << ent.x () << ", " << ent.y () << "\n";
          {
            auto cloned = state->clone ();
            cloned->entities = *from;
            cloned->cmd_budget += cnt - 1;
            std::array<int, 3> cmd = {ent.x (), ent.y (), 1};
            cloned->cmds.emplace_back (cmd);
            const int cell = cloned->board.cell (ent.x (), ent.y ());
            const int rcell = rotate_cell_right (cell);
            // std::cerr << " fork at " << ent.x () << "," << ent.y () << " "
            //           << cell << " -> " << rcell << "\n";
            cloned->board.cell (ent.x (), ent.y ()) = -rcell;
            bag.push_back (cloned);
          }
          if (cell >= 6) {
            {
              auto cloned = state->clone ();
              cloned->entities = *from;
              cloned->cmd_budget += cnt - 1;
              std::array<int, 3> cmd = {ent.x (), ent.y (), -1};
              cloned->cmds.emplace_back (cmd);
              const int cell = cloned->board.cell (ent.x (), ent.y ());
              const int rcell = rotate_cell_left (cell);
              // std::cerr << " fork at " << ent.x () << "," << ent.y () << " "
              //           << cell << " -> " << rcell << "\n";
              cloned->board.cell (ent.x (), ent.y ()) = -rcell;
              bag.push_back (cloned);
            }
            if (state->cmd_budget + cnt >= 2) {
              auto cloned = state->clone ();
              cloned->entities = *from;
              std::array<int, 3> cmd = {ent.x (), ent.y (), 1};
              cloned->cmds.emplace_back (cmd);
              cloned->cmds.emplace_back (cmd);
              const int cell = cloned->board.cell (ent.x (), ent.y ());
              const int rcell = rotate_cell_right (rotate_cell_right (cell));
              // std::cerr << " fork at " << ent.x () << "," << ent.y () << " "
              //           << cell << " -> " << rcell << "\n";
              cloned->board.cell (ent.x (), ent.y ()) = -rcell;
              bag.push_back (cloned);
            }
          }
        }
      }

      temp.pack (mask, *to);
      if (mask[0] == MOVE_BAD) {
        // std::cerr << " ###ending choice " << choices_traced
        //           << " as move result is " << mask[0] << "\n";
        break;
      }
    }
  }
  std::cerr << "move search failed, " << choices_traced << " steps\n";
  return {};
}

void Solver::step (std::istream& in)
{
  Entities entities;
  entities.read (in);

  auto [x, y, cmd] = solve (State::create (board, entities));

  constexpr bool log484 = false;
  if (cmd == 0) {
    std::cout << "WAIT" << std::endl;
  } else if (cmd > 0) {
    std::cout << x << " " << y << " RIGHT" << std::endl;
    board.cell (x, y) = rotate_cell_right (board.cell (x, y));
    if (log484) {
      std::cerr << "changed cell at " << x << ", " << y << " to "
                << (int)board.cell (x, y) << "\n";
    }
  } else {
    std::cout << x << " " << y << " LEFT" << std::endl;
    board.cell (x, y) = rotate_cell_left (board.cell (x, y));
    if (log484) {
      std::cerr << "changed cell at " << x << ", " << y << " to "
                << (int)board.cell (x, y) << "\n";
    }
  }
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
  const auto max_step = 4;
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
static const std::string test_input_string_old = R"(11 10
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
2 1 LEFT
1
8 1 TOP
3 1 LEFT
1
8 2 TOP
)";

static const std::string test_input_string = R"(6 6
0 -3 -3 -5 1 -1
-13 11 4 0 -3 1
11 6 1 0 10 1
0 0 10 3 3 1
0 0 0 0 0 3
0 0 0 0 0 -3
5
1 0 TOP
2
2 0 TOP
0 1 LEFT
1 1 TOP
2
2 1 TOP
0 2 TOP
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
  return movement_masks[std::abs (cell)];
}

static const std::array<int, 14> soft_masks = [] {
  std::array<int, 14> array;
  array.fill (0);
  for (int i = 0; i < 14; ++i) {
    int c = i;
    for (int j = 0; j < 4; ++j) {
      array[i] |= movement_masks[c];
      c = rotate_cell_right (c);
    }
  }
  return array;
}();

static int cell_movement_mask_soft (int cell)
{
  return soft_masks[std::abs (cell)];
}
//}}END STABLE
