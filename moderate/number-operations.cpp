#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
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
      "44 6 1 49 47",
      "34 1 49 2 21",
      "31 38 27 51 18",
    };
    std::vector<std::string> v_expect {
      "NO",
      "YES",
      "NO",
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

  bool ntest2(const std::vector<int> &v) {
    int n = 1;
    for(int i = 1; i < v.size(); ++i) {
      n *= 3;
    }
    for(int i = 0; i < n; ++i) {
      int acc = v[0];
      for(int o = i, j = 1; j < v.size(); ++j) {
        switch(o % 3) {
          case 0: acc = acc - v[j]; break;
          case 1: acc = acc + v[j]; break;
          case 2: acc = acc * v[j]; break;
        }
        o /= 3;
      }
      if(42 == acc) {
        return true;
      }
    }
    return false;
  }

  bool ntest(const std::vector<int> &v) {
    std::vector<int> v2(v);
    std::sort(v2.begin(), v2.end());
    while(std::next_permutation(v2.begin(), v2.end())) {
      if(ntest2(v2)) {
        return true;
      }
    }
    return false;
  }

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    std::istringstream ss { line };
    std::vector<int> v(5);
    for(int i = 0; i < 5; ++i) {
      ss >> v[i];
    }
    std::cout << (ntest(v) ? "YES\n" : "NO\n");
  }
}
