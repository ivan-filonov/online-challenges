#include <fstream>
#include <iostream>
#include <vector>

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
      "4; 1:[1, 3, 2], 2:[1], 3:[4, 3], 4:[4, 3]",
      "3; 1:[1, 3, 2], 2:[1], 3:[1]",
    };
    std::vector<std::string> v_expect {
      "Yes",
      "No",
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

  struct state {
    int N;
    std::vector<std::vector<int>> v;

    void setN (int);
    void setUid(int);
    void addPlace(int);

    bool run();
  };

  void parse_state(const std::string &line, state &st) {
    size_t pos = 0;
    st.setN(std::stoi(line, &pos));

    while(pos < line.length() && !std::isdigit(line[pos])) {
      ++pos;
    }

    for(;pos < line.length();) {
      int uid = 0;
      while(std::isdigit(line[pos])) {
        uid = uid * 10 + (line[pos] - '0');
        ++pos;
      }
      if(line[pos++] != ':') {
        throw std::exception();
      }
      if(line[pos++] != '[') {
        throw std::exception();
      }
      st.setUid(uid);

      for(;line[pos] != ']';) {
        int placeid = 0;
        for(; std::isdigit(line[pos]); ++pos) {
          placeid = placeid * 10 + (line[pos] - '0');
        }
        st.addPlace(placeid);
        if(',' == line[pos]) {
          while(' ' == line[++pos]);
        }
      }
      ++pos;
      if(pos + 2 >= line.length()) {
        break;
      }
      if(line[pos] != ',') {
        throw std::exception();
      }
      while(line[++pos] == ' ');
    }
  }

  void process(std::string line) {
#ifdef TEST
    // N; Uid:[Seats ','] ','
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    
    state st;
    parse_state(line, st);
    std::cout << (st.run() ? "Yes\n" : "No\n");
  }

  void state::addPlace(int placeid) {
    v.back().push_back(placeid);
  }

  void state::setN(int N_) {
    N = N_;
  }

  void state::setUid(int uid) {
    v.emplace_back();
  }

  bool state::run() {
    uint64_t c = 1;
    for(const auto &vv : v) {
      c *= vv.size();
    }
    if(c > 10000000000ull) {
        throw std::exception();
    }

    bool fail = true;
    for(uint64_t i = 0; i != c && fail; ++i) {
      auto ii = i;
      uint64_t mask = 0;
      fail = false;
      for(const auto &vv : v) {
        auto seat = 1ull << vv[ii % vv.size()];
        ii /= vv.size();
        if(mask & seat) {
          fail = true;
          break;
        }
        mask |= seat;
      }
    }
    return !fail;
  }
}
