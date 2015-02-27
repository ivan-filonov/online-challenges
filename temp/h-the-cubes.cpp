#include <algorithm>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_set>
#include <vector>

#define TEST

namespace {
#ifdef TEST
  void test();
#endif //#ifdef TEST
  void process_file(char*);
}

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  void process(std::string s);

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "5;******   ** ****   *** ********  o**o* ** * ************   ** * **o* ************ o **** **   ******** ***o  ****o**o  ******",
      "5;****** * ** * **   *** ********o*o** *o**   ************   **o* ** *o************   **** **   ******** *** o ****o**  o******",
      "7;********     ** *** ** * * ** * * **     **** *********** *   ** *** **   *o**o***o**oo   ****************  oo ** * * ** * * ** * * ** *o o****************     ** * ****o* * ** *** **o    ****************     ** *** **     ****** ** o  o**************** *oo ** *** **o  * ** *** **o    *********** ****   * ** * * ** * *o** ***o**     ********",
    };
    std::vector<std::string> v_expect {
      "15",
      "0",
      "39",
    };
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      process(v_test[i]);
      std::cout << v_expect[i] << "\n";
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      process(line);
    }
  }

  struct work_unit {
    size_t edge;
    std::vector<char> m3;
    std::vector<int> d3;
    const size_t npos = std::numeric_limits<size_t>::max();

    // copy cube representation into destination and init distances for
    // allowed positions to backing array size.
    work_unit(const std::string& line) {
      size_t delim;
      edge = std::stoi(line, &delim);
      m3.resize(edge * edge * edge);
      std::copy(line.begin() + delim + 1, line.end(), m3.begin());

      for(int y = 0; y < edge; ++y) {
        for(int x = 0; x < edge; ++x) {
          std::cout << m3[ofs(x,y,0)];
        }
        std::cout << "\n";
      }

      d3.resize(m3.size(), std::numeric_limits<int>::max());
      for(size_t i = 0, j = d3.size(); i < j; ++i) {
        if(' ' == m3[i] || 'o' == m3[i]) {
          d3[i] = j;
        }
      }
    }

    // edge**2 * z + edge * y + x
    size_t ofs (int x, int y, int z) const {
      return (z * edge + y) * edge + x;
    }

    // all coords are non-negative and less than edge length
    bool inside(int x, int y, int z) const {
      return x >= 0 && y >= 0 && z >= 0 && x < edge && y < edge && z < edge;
    }

    // move to ajacent cell
    size_t shift(size_t pos, int dx, int dy, int dz) const {
      if(std::abs(dx) + std::abs(dy) + std::abs(dz) > 1) {
        throw std::exception();
      }
      int x,y,z;
      std::tie(x,y,z) = coords(pos);
      x += dx;
      y += dy;
      z += dz;
      return inside(x,y,z) ? ofs(x,y,z) : npos;
    }

    // move by +x, -x, +y, -y, +z, -z
    size_t shift(size_t pos, int dir) const {
      if(dir < 0 || dir > 5) {
        throw std::exception();
      }
      int d[3] = {0, 0, 0};
      d[dir/2] = (dir & 1) ? -1 : 1;
      return shift(pos, d[0], d[1], d[2]);// 0:x+ 1:x- 2:y+ 3:y- 4:z+ 5:z-
    }

    // can move if destination is not wall and if changing floor than
    // either origin or destination is hole.
    bool can_move (size_t pos, int dir) const {
      auto dst = shift(pos, dir);
      if(npos == dst) {
        return false;
      }
      // cannot move into wall
      bool res = m3[dst] != '*';
      if(res && dir > 3) {
        res = ('o' == m3[pos]) || ('o' == m3[dst]);
      }
      return res;
    }

    std::tuple<int,int,int> coords(size_t ofs) const {
      auto x = ofs % edge;
      ofs /= edge;
      auto y =  ofs % edge;
      ofs /= edge;
      return std::make_tuple<int,int,int>(x, y, ofs);
    }

    const int & dat(int x, int y, int z) const{
      return d3[ofs(x,y,z)];
    }

    int & dat(int x, int y, int z) {
      return d3[ofs(x,y,z)];
    }

    const char & mat(int x, int y, int z) const{
      return m3[ofs(x,y,z)];
    }

    char & mat(int x, int y, int z) {
      return m3[ofs(x,y,z)];
    }

    size_t get_gate(int z) {
      for(int t = 0; t < edge; ++t) {
        if('*' != mat(t, 0, z)) {
          return ofs(t, 0, z);
        }
        if('*' != mat(t, edge - 1, z)) {
          return ofs(t, edge - 1, z);
        }
        if('*' != mat(0, t, z)) {
          return ofs(0, t, z);
        }
        if('*' != mat(edge - 1, t, z)) {
          return ofs(edge - 1, t, z);
        }
      }
      return npos;
    }

    std::unordered_set<size_t> border;
    void run() {
      auto p_from = get_gate(0);
      auto p_to = get_gate(edge - 1);
      d3[p_from] = 0;
      border.insert(p_from);

      while(!border.empty()) {
        step();
      }

      std::cout << d3[p_to] << "\n";
    }

    void step() {
      size_t pt = *border.begin();
      int ptd = d3[pt];
      for(auto p : border) {
        if(d3[p] < ptd) {
          pt = p;
          ptd = d3[p];
        }
      }
      border.erase(border.find(pt));
      for(int dir = 0; dir < 6; ++dir) {
        if(!can_move(pt, dir)) {
          continue;
        }
        auto p = shift(pt, dir);
        if(d3[p] == d3.size()) {
          border.insert(p);
        }
        if(d3[p] > ptd) {
          d3[p] = ptd + 1;
        }
      }
    }
  };

  void process(std::string line) {
    std::cout << "s = '" << line << "'\n";
    work_unit u { line };
    u.run();
  }
}
