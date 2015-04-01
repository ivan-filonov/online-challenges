//#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <tuple>
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
    const std::string s;
    size_t pos;

    state(const std::string &s_) : s(s_), pos(0) {}
    
    int p_int() {
      int r = 0;
      while(pos < s.length() && std::isdigit(s[pos])) {
        r = r * 10 + (s[pos] - '0');
        ++pos;
      }
      return r;
    }

    void p_expect(const char c) {
      if(pos < s.length() && c == s[pos]) {
        ++pos;
      } else {
        throw std::exception();
      }
    }

    bool p_skip(const char c) {
      if(pos < s.length() && c == s[pos]) {
        ++pos;
        return true;
      }
      return false;
    }

    void parse() {
      setN( p_int() );
      p_expect(';');
      p_expect(' ');
      for(;;) {
        addUid( p_int() );
        p_expect(':');
        p_expect('[');
        while(std::isdigit(s[pos])) {
          addSid( p_int() );
          if(p_skip(',')) {
            p_expect(' ');
          }
        }
        p_expect(']');
        if(p_skip(',')) {
          p_expect(' ');
        } else {
          break;
        }
      }
    }
    
    struct entity {
      int uid;
      std::bitset<64> mask;

      entity() {
      }
    };

    std::vector<entity> v;
    
    void setN(int n) {
      v.clear();
      v.reserve(n);
    }

    void addUid(int uid_) {
      v.emplace_back();
      v.back().uid = uid_;
    }

    void addSid(int sid) {
      v.back().mask.set(sid - 1, true);
    }

    void pre_sort_entities() {
      for(size_t i = 0, imax = v.size() - 1, jmax = v.size(); i != imax; ++i) {
        for(size_t j = i + 1; j != jmax; ++j) {
          if(v[j].mask.count() < v[i].mask.count()) {
            std::swap(v[i], v[j]);
          }
        }
      }
//      std::sort(v.begin(), v.end(), [](const entity &e1, const entity &e2) { return e1.mask.count() < e2.mask.count(); });
    }
    
    bool run() {
      parse();
      pre_sort_entities();
      for(uint64_t t = 3, lim = 1ull << v.size(); t != lim; ++t) {
        if(!(t&(t-1))) {
          continue;
        }
        std::bitset<64> m;
        for(size_t o = 0; o < v.size(); ++o) {
          m |= (t & (1ull << o)) ? v[o].mask : 0;
        }
        std::bitset<64> u { t };
        if(m.count() < u.count()) {
          return false;
        }
      }
      return true;
    }
  };

  void process(std::string line) {
#ifdef TEST
    // N; Uid:[Seats ','] ','
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST

    state st(line);
    std::cout << (st.run() ? "Yes\n" : "No\n");
  }
}
