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
#else
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  void process(std::string s);

#ifdef TEST
  void test() {
    std::vector<std::string> v_test { "12", "123", "1313" };
    std::vector<std::string> v_expect { "2", "3", "1" };
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

  int scan(const std::string &s, int pos) {
    if(s.length() - pos < 2) {
      return 1;
    }
    return scan(s, pos + 1) + scan(s, pos + 2);
  }

  int scan2(const std::string &s, int pos) {
    int a = 1, b = 1;
    for(int i = 0; i < s.length() ; ++i) {
      int c = a + b;
      a = b;
      b = c;
    }
    return a;
  }

  void process(std::string line) {
    std::vector<std::string> v;
    for(bool flag = true; flag && !line.empty();) {
      flag = false;
      size_t i = 0, j = line.length() - 1;
      for(; i != j; ++i) {
        if(line[i] > '2' || ('2' == line[i] && line[i+1] > '6')) {
          flag = true;
          break;
        }
      }
      if(flag) {
        v.emplace_back( line.substr(0, i + 1) );
        line.erase(0, i + 1);
      }
    }
    if(!line.empty()) {
      v.emplace_back( std::move(line) );
    }
    int r2 = 1;
    for(auto &s : v) {
      r2 *= scan2(s, 0);
    }
    std::cout << r2 << "\n";
  }
}
