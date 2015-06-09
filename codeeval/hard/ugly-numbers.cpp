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
      "1",
      "9",
      "011",
      "12345",
    };
    std::vector<std::string> v_expect {
      "0",
      "1",
      "6",
      "64",
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

  bool probe(const std::string &s, int p) {
    int accum = 0;
    int current = s[0] - '0';
    int sign = 1;
    for(int i = 1; i < s.length(); ++i) {
      const auto op = p % 3;
      p /= 3;
      const int v = s[i] - '0';
      switch(op) {
        case 0:
          current = current * 10 + v;
          break;
        case 1:
          accum += sign * current;
          sign = 1;
          current = v;
          break;
        case 2:;
          accum += sign * current;
          sign = -1;
          current = v;
          break;
      }
    }
    accum += sign * current;
    return accum % 2 && accum % 3 && accum % 5 && accum % 7;
  }

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    int n = 1;
    for(int i = 1; i < line.length(); ++i) {
      n *= 3;
    }
    int res = 0;
    for(int j = 0; j < n; ++j) {
      if(probe(line, j)) {
        ++res;
      }
    }
    std::cout << (n - res) << "\n";
  }
}
