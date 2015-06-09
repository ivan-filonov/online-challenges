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
      "0 0 1 5",
      "12 13 12 13",
      "0 1 0 5",
    };
    std::vector<std::string> v_expect {
      "NE",
      "here",
      "N",
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

  int s2i (int x) {
    if(x > 0) {
      return 1;
    }
    if(x < 0) {
      return 2;
    }
    return 0;
  }

  std::vector<std::string> dir {
    "here",    "N",    "S",
    "E",      "NE",    "SE",
    "W",      "NW",    "SW",
  };
  
  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    std::istringstream ss { line };
    int O, P, Q, R;
    ss >> O >> P >> Q >> R;
    Q -= O;
    R -= P;
    auto idx = s2i(Q) * 3 + s2i(R);
    std::cout << dir[idx] << "\n";
  }
}
