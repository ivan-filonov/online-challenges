#include <array>
#include <fstream>
#include <iostream>
#include <vector>

//#define TEST

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
  
  struct board_t {
    const std::string s;
    const int wid;
    const int hei;

    const char at(int row, int column) const {
      return s[row * wid + column];
    }

    const char at(size_t pos) const {
      return s[pos];
    }

    char operator [] (size_t pos) const {
      return s[pos];
    }

    bool test(int x, int y) const {
      return x >= 0 && y >= 0 && x < wid && y < hei;
    }

    board_t(const std::string& _s, int _w, int _h) : s(_s), wid(_w), hei(_h) { }
  }
  const board { "ABCE" "SFCS" "ADEE", 4, 3 };

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "ASADB",
      "ABCCED",
      "ABCF",
      "ASADFCEESECB",
      "ASADFCEESECE",
      "EESECBASADFC",
      "EESECBASADFG",
      "ECF",
    };
    std::vector<std::string> v_expect {
      "False",
      "True",
      "False",
      "True",
      "False",
      "True",
      "False",
      "True",
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

  struct state_t {
    const std::string s;

    std::array<bool,12> avail;

    state_t (const std::string &s) : s(s) {
    }

    bool run() {
      if(s.length() > board.s.length()) {
        return false;
      }
      if(s.find_first_not_of(board.s) != std::string::npos) {
        return false;
      }

      for(int y = 0; y < board.hei; ++y) {
        for(int x =0 ; x < board.wid; ++x) {
          avail.fill(true);
          if(scan(x, y, 0)) {
            return true;
          }
        }
      }

      return false;
    }

    bool scan(int x, int y, int sp) {
      const size_t pos = x + board.wid * y;

      if(!board.test(x, y) || board[pos] != s[sp] || !avail[pos]) {
        return false;
      }
     
      ++sp;
      if(s.length() == sp) {
        return true;
      }
      
      avail[pos] = false;

      for(int dir = 0; dir < 4; ++dir) {
        int d[2] = { 0, 0 };
        d[dir/2] = (dir&1) ? 1 : -1;
        if(scan(x + d[0], y + d[1], sp)) {
          return true;
        }
      }

      avail[pos] = true;

      return false;
    }
  };

  void process(std::string line) {
#ifdef TEST
    std::cout << line << "\n";
#endif //#ifdef TEST
    state_t st { line };
    std::cout << (st.run() ? "True\n" : "False\n");
  }
}
