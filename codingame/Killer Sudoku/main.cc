#undef _GLIBCXX_DEBUG // disable run-time bound checking, etc
#pragma GCC optimize("Ofast,inline") // Ofast = O3,fast-math,allow-store-data-races,no-protect-parens

#pragma GCC target("bmi,bmi2,lzcnt,popcnt") // bit manipulation
#pragma GCC target("movbe")                 // byte swap
#pragma GCC target("aes,pclmul,rdrnd")      // encryption
#pragma GCC target("avx,avx2,f16c,fma,sse3,ssse3,sse4.1,sse4.2") // SIMD

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <ios>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

class Solver
{
public:
  virtual void read (std::istream& in) = 0;
  virtual void write (std::ostream& out) = 0;
  virtual bool solve () = 0;
};

namespace v1
{
struct Cell {
  int           value = 0;
  struct Cage*  cage;
  struct Group* group;
  struct Group* col;
  struct Group* row;

  int masks () const;
};

struct Cage {
  int value = 0;
  int sum = 0;
  int mask = 0;
  int undefs = 0;
};

struct Group {
  int sum = 0;
  int mask = 0;
  int undefs = 0;
};

int Cell::masks () const
{
  return cage->mask | group->mask | col->mask | row->mask;
}

struct State : public Solver {
  // read from inputgg
  Cell cells[81];
  Cage cages[128];

  // other
  Group groups[9];
  Group rows[9];
  Group cols[9];

  int total_undefs = 0;

  void read (std::istream& in) override
  {
    std::string grid_line, cages_line;
    for (int y = 0; y < 9; ++y) {
      in >> grid_line >> cages_line;
      in.ignore ();
      for (int x = 0; x < 9; ++x) {
        const auto cell_id = y * 9 + x;
        const int  value = grid_line[x] == '.' ? 0 : (grid_line[x] - '0');
        const int  bit = value > 0 ? 1 << value : 0;
        Cell*      cell = &cells[cell_id];
        cell->value = value;
        const int cage_id = cages_line[x];
        Cage*     cage = &cages[cage_id];
        cell->cage = cage;
        cage->sum += value;
        cage->mask |= bit;
        const int group_id = (y / 3) * 3 + x / 3;
        Group*    group = &groups[group_id];
        cell->group = group;
        group->sum += value;
        group->mask |= bit;
        if (0 == value) {
          cage->undefs += 1;
          group->undefs += 1;
          total_undefs += 1;
        }

        Group* col = &cols[x];
        cell->col = col;
        col->mask |= bit;
        col->sum += value;

        Group* row = &rows[y];
        cell->row = row;
        row->mask |= bit;
        row->sum += value;
      }
    }
    std::string cage_values_str;
    std::getline (in, cage_values_str);
    std::stringstream ss{cage_values_str};
    for (std::string s; ss >> s;) {
      cages[s[0]].value = std::stoi (s.substr (2));
    }
  }

  void write (std::ostream& out) override
  {
    for (int y = 0; y < 9; ++y, out << "\n") {
      for (int x = 0; x < 9; ++x) {
        const auto v = cells[y * 9 + x].value;
        out << (v ? (char)('0' + v) : '.');
      }
    }
  }

  bool put (Cell* cell, int value)
  {
    if (value < 1 || value > 9) {
      return false;
    }
    const int bit = 1 << value;
    Group*    row = cell->row;
    if (row->mask & bit) {
      return false;
    }
    Group* col = cell->col;
    if (col->mask & bit) {
      return false;
    }

    Cage* cage = cell->cage;
    if (cage->mask & bit) {
      return false;
    }
    Group* group = cell->group;
    if (group->mask & bit) {
      return false;
    }
    if (cage->undefs == 1) {
      if (cage->sum + value != cage->value) {
        return false;
      }
    } else {
      if (cage->sum + value >= cage->value) {
        return false;
      }
    }

    cell->value = value;

    col->sum += value;
    col->mask |= bit;

    row->sum += value;
    row->mask |= bit;

    cage->mask |= bit;
    cage->sum += value;
    cage->undefs -= 1;

    group->mask |= bit;
    group->sum += value;
    group->undefs -= 1;

    total_undefs -= 1;
    return true;
  }

  void remove (Cell* cell)
  {
    const int value = cell->value;
    const int bit = 1 << value;
    Group*    row = cell->row;
    Group*    col = cell->col;
    Cage*     cage = cell->cage;
    Group*    group = cell->group;

    cell->value = 0;

    col->sum -= value;
    col->mask &= ~bit;

    row->sum -= value;
    row->mask &= ~bit;

    cage->mask &= ~bit;
    cage->sum -= value;
    cage->undefs += 1;

    group->mask &= ~bit;
    group->sum -= value;
    group->undefs += 1;

    total_undefs += 1;
  }

  bool solve () override
  {
    if (total_undefs == 0) {
      return true;
    }

    std::vector<Cell*> changed;
    // 1. process all groups with undef = 1
    for (;;) {
      bool stop = true;
      bool bad_branch = false;

      for (int cell_id = 0; cell_id < 81; ++cell_id) {
        Cell* cell = &cells[cell_id];
        if (cell->value) {
          continue;
        }
        Cage* cage = cell->cage;
        if (cage->undefs == 1) {
          const int value = cage->value - cage->sum;
          if (put (cell, value)) {
            changed.push_back (cell);
            stop = false;
            break;
          } else {
            bad_branch = true;
            break;
          }
        }
        Group* group = cell->group;
        if (group->undefs == 1) {
          const int value = 45 - group->sum;
          if (put (cell, value)) {
            changed.push_back (cell);
            stop = false;
            break;
          } else {
            bad_branch = true;
            break;
          }
        }
        auto row = cell->row;
        if (8 == __builtin_popcount (row->mask)) {
          const int value = 45 - row->sum;
          if (put (cell, value)) {
            changed.push_back (cell);
            stop = false;
            break;
          } else {
            bad_branch = true;
            break;
          }
        }
        auto col = cell->col;
        if (8 == __builtin_popcount (col->mask)) {
          const int value = 45 - col->sum;
          if (put (cell, value)) {
            changed.push_back (cell);
            stop = false;
            break;
          } else {
            bad_branch = true;
            break;
          }
        }
        const int mask = 0x3fe & ~cell->masks ();
        if (1 == __builtin_popcount (mask)) {
          for (int value = 9; value >= 1; --value) {
            if (mask == (1 << value)) {
              if (put (cell, value)) {
                changed.push_back (cell);
                stop = false;
                break;
              } else {
                bad_branch = true;
                break;
              }
            }
          }
        }
      }

      if (bad_branch) {
        for (Cell* cell : changed) {
          remove (cell);
        }
        return false;
      }

      if (stop) {
        break;
      }
    }

    if (total_undefs == 0) {
      return true;
    }

    // 2. process one item
    int p2_count = 0;
    for (int cell_id = 0; cell_id < 81; ++cell_id) {
      Cell* cell = &cells[cell_id];
      if (cell->value) {
        continue;
      }
      p2_count += 1;
      bool      bad_branch = true;
      const int mask = cell->masks ();
      for (int value = 9; value >= 1; --value) {
        if (mask & (1 << value)) {
          continue;
        }
        if (put (cell, value)) {
          bad_branch = false;
          if (solve ()) {
            return true;
          }
          remove (cell);
        }
      }
      if (bad_branch) {
        break;
      }
    }

    for (Cell* cell : changed) {
      remove (cell);
    }

    return p2_count + changed.size () == 0;
  }
};
} // namespace v1

namespace v2
{

struct Group {
  int mask = 0;
  int sum = 0;
  int value = 45;
  int undefs = 0;

  bool can_add (int new_value) const
  {
    const int bit = 1 << new_value;
    if (mask & bit) {
      return false;
    }

    if (undefs == 1) {
      return sum + new_value == value;
    } else {
      return sum + new_value < value;
    }
  }

  void set (int new_value)
  {
    mask |= 1 << new_value;
    sum += new_value;
  }

  void add (int new_value)
  {
    set (new_value);
    undefs -= 1;
  }

  void remove (int new_value)
  {
    mask &= ~(1 << new_value);
    sum -= new_value;
    undefs += 1;
  }
};

struct State : public Solver {
  int8_t cell_values[81];
  int8_t cell_cages[81];

  Group rows[9];
  Group cols[9];
  Group groups[9];
  Group cages[64];

  void read (std::istream& in) override
  {
    std::string grid_line, cages_line;
    for (int y = 0; y < 9; ++y) {
      in >> grid_line >> cages_line;
      in.ignore ();
      for (int x = 0; x < 9; ++x) {
        const int value = (grid_line[x] != '.') ? grid_line[x] - '0' : 0;
        const int cage = cages_line[x] - 'A';
        const int ofs = y * 9 + x;
        const int group = (y / 3) * 3 + x / 3;

        cell_values[ofs] = value;
        cell_cages[ofs] = cage;

        if (value > 0) {
          rows[y].set (value);
          cols[x].set (value);
          groups[group].set (value);
          cages[cage].set (value);
        } else {
          rows[y].undefs += 1;
          cols[x].undefs += 1;
          groups[group].undefs += 1;
          cages[cage].undefs += 1;
        }
      }
    }
    std::string cage_values_str;
    std::getline (in, cage_values_str);
    std::stringstream ss{cage_values_str};
    for (std::string s; ss >> s;) {
      const int cage = s[0] - 'A';
      const int value = std::stoi (s.substr (2));
      cages[cage].value = value;
    }
  }

  void write (std::ostream& out) override
  {
    for (int y = 0; y < 9; ++y) {
      if (y) {
        out << "\n";
      }
      for (int x = 0; x < 9; ++x) {
        const int  ofs = y * 9 + x;
        const char c = cell_values[ofs] ? cell_values[ofs] + '0' : '.';
        out << c;
      }
    }
  }

  bool put (int ofs, int value)
  {
    if (value < 1 || value > 9 || cell_values[ofs] != 0) {
      return false;
    }
    const int y = ofs / 9;
    if (!rows[y].can_add (value)) {
      return false;
    }
    const int x = ofs % 9;
    if (!cols[x].can_add (value)) {
      return false;
    }
    const int g = (y / 3) * 3 + x / 3;
    if (!groups[g].can_add (value)) {
      return false;
    }
    const int cage = cell_cages[ofs];
    if (!cages[cage].can_add (value)) {
      return false;
    }
    cell_values[ofs] = value;
    rows[y].add (value);
    cols[x].add (value);
    groups[g].add (value);
    cages[cage].add (value);
    return true;
  }

  void remove (int ofs)
  {
    const int value = cell_values[ofs];
    cell_values[ofs] = 0;
    const int y = ofs / 9;
    rows[y].remove (value);
    const int x = ofs % 9;
    cols[x].remove (value);
    const int g = (y / 3) * 3 + x / 3;
    groups[g].remove (value);
    const int cage = cell_cages[ofs];
    cages[cage].remove (value);
  }

  int cells[81];
  int cells_locked = 0;

  void push_cells_index (int i)
  {
    if (i != cells_locked) {
      std::swap (cells[i], cells[cells_locked]);
    }
    cells_locked += 1;
  }

  bool solve_r ()
  {
    if (cells_locked == 81) {
      return true;
    }

    for (int i = cells_locked; i < 81; ++i) {
      const int ofs = cells[i];
      const int y = ofs / 9;
      const int x = ofs % 9;
      const int g = (y / 3) * 3 + x / 3;
      const int c = cell_cages[ofs];

      int value = 0;
      if (cages[c].undefs == 1) {
        value = cages[c].value - cages[c].sum;
      } else if (groups[g].undefs == 1) {
        value = 45 - groups[g].sum;
      } else if (rows[y].undefs == 1) {
        value = 45 - rows[y].sum;
      } else if (cols[x].undefs == 10) {
        value = 45 - cols[x].sum;
      }
      if (value > 0) {
        if (put (ofs, value)) {
          push_cells_index (i);
          if (solve_r ()) {
            return true;
          } else {
            remove (ofs);
            cells_locked -= 1;
            return false;
          }
        } else {
          return false;
        }
      }

      const int mask_used
        = cages[c].mask | groups[g].mask | rows[y].mask | cols[x].mask;
      const int mask_avl = 0x3fe & ~mask_used;
      const int num_avl = __builtin_popcount (mask_avl);
      if (0 == num_avl) {
        return false;
      }
      if (1 == num_avl) {
        value = 1;
        while (mask_avl != 1 << value) {
          value += 1;
        }
        if (put (ofs, value)) {
          push_cells_index (i);
          if (solve_r ()) {
            return true;
          } else {
            remove (ofs);
            cells_locked -= 1;
            return false;
          }
        } else {
          return false;
        }
      }
    }

    const int ofs = cells[cells_locked];
    const int y = ofs / 9;
    const int x = ofs % 9;
    const int g = (y / 3) * 3 + x / 3;
    const int c = cell_cages[ofs];
    const int mask_used
      = cages[c].mask | groups[g].mask | rows[y].mask | cols[x].mask;
    const int mask_avl = 0x3fe & ~mask_used;
    for (int value = 1; value <= 9; ++value) {
      if (mask_avl & (1 << value)) {
        if (put (ofs, value)) {
          push_cells_index (cells_locked);
          if (solve_r ()) {
            return true;
          } else {
            remove (ofs);
            cells_locked -= 1;
            return false;
          }
        }
      }
    }

    return false;
  }

  bool solve () override
  {
    for (int i = 0; i < 81; ++i) {
      cells[i] = i;
      if (cell_values[i] > 0) {
        push_cells_index (i);
      }
    }
    return solve_r ();
  }
};

} // namespace v2


int main ()
{
  using namespace v1;
#ifndef TEST
  auto& in = std::cin;
#else
  constexpr int test_id = 0;

  const std::string test_ins[] = {
    R"X(56..1..2. aabbccdde
..72..68. afghhiide
..2.87.15 jfggklmme
......3.9 jjgnklopp
.7....2.. qqgnooorr
9.634.8.. stuuvwwxx
2.9..8... stuuvvwyz
..41.2... sAuuByyyz
.8.4...3. CADDBEEFF
a=12 b=17 c=4 d=14 e=15 f=13 g=19 h=7 i=10 j=16 k=10 l=13 m=10 n=15 o=15 p=13 q=11 r=11 s=18 t=3 u=28 v=15 w=20 x=8 y=22 z=12 A=11 B=13 C=6 D=9 E=10 F=5)X",
    R"X(........1 abbcdddef
....5.... agccchfff
...3..... ijjjhhkkl
.......9. ijmmnnokp
..6...... qrrsttuvv
....9.7.. qwxsyuuzA
.....3... qwxxyBBCA
......... qDxEEBBCC
..8..5... DDFFFBGHH
a=14 b=9 c=17 d=22 e=3 f=20 g=3 h=9 i=6 j=21 k=19 l=7 m=9 n=11 o=4 p=6 q=16 r=15 s=11 t=11 u=10 v=9 w=9 x=20 y=16 z=5 A=5 B=22 C=19 D=17 E=11 F=15 G=3 H=11)X",
  };
  std::stringstream in{test_ins[test_id]};
#endif

  State state;
  state.read (in);
  state.solve ();

#ifndef TEST
  auto& out = std::cout;
#else
  std::stringstream out;
#endif
  state.write (out);

#ifdef TEST
  const std::string test_outs[] = {
    R"(568913427
197254683
342687915
851726349
473891256
926345871
219538764
734162598
685479132)",
    R"(568913427
197254683
342687915
851726349
473891256
926345871
219538764
734162598
685479132)",
  };
  const auto& expected = test_outs[test_id];

  std::string outs = out.str ();
  while (!outs.empty () && std::isspace (outs.back ())) {
    outs.pop_back ();
  }
  if (expected != outs) {
    std::cout << "FAILED:\nresult:\n"
              << out.str () << "\n\nexpected:\n"
              << expected << "\n";
  } else {
    std::cout << "PASSED\n";
  }
#endif
  std::cout << "done." << std::endl;
  return 0;
}
