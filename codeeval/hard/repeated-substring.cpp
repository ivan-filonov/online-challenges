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
      "banana",
      "am so uniqe",
    };
    std::vector<std::string> v_expect {
      "an",
      "NONE",
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

  struct res {
    int pos1;
    int pos2;
    int len;
    res(int _p1, int _p2, int _l) : pos1(_p1), pos2(_p2), len(_l) {}
  };

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    std::vector<std::vector<size_t>> cp;
    for(size_t i = 0; i != line.length(); ++i) {
      const auto c = line[i];
      if(cp.size() <= c) {
        cp.resize(c + 1);
      }
      cp[c].push_back(i);
    }
    std::vector<res> rv;
    size_t rvm = 0;
    for(auto &v : cp) {
      if(v.size() < 2) {
        continue;
      }
      for(int j1 = 0; j1 < v.size() - 1; ++j1) {
        const auto pos1 = v[j1];
        for(int j2 = j1 + 1; j2 < v.size(); ++j2) {
          const auto pos2 = v[j2];
          size_t o = 0;
          bool spaces = true;
          while(line[pos1 + o] == line[pos2 + o] && pos1 + o != pos2) {
            spaces &= ' ' == line[pos1 + o];
            spaces &= ' ' == line[pos2 + o];
            ++o;
          }
          if(!spaces) {
            rv.emplace_back(pos1, pos2, o);
            rvm = std::max(rvm, o);
          }
        }
      }
    }
    for(size_t t = 0; t != rv.size();) {
      if(rv[t].len == rvm) {
        ++t;
      } else {
        rv.erase(rv.begin() + t);
      }
    }
    size_t i = 0;
    if(!rv.empty()) {
      for(size_t t = 0; t != rv.size(); ++t) {
        if(rv[t].pos1 < rv[i].pos1) {
          i = t;
        }
      }
    }
    std::cout << (rv.empty() ? "NONE" : line.substr(rv[i].pos1, rv[i].len)) << "\n";
  }
}
