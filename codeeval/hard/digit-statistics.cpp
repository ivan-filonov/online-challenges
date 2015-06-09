#include <fstream>
#include <iostream>
#include <sstream>
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
"2 5"
    };
    std::vector<std::string> v_expect {
"0: 0, 1: 0, 2: 2, 3: 0, 4: 1, 5: 0, 6: 1, 7: 0, 8: 1, 9: 0"
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

  void process(std::string line) {
    std::istringstream ss { line };
    int a;
    long long n;
    ss >> a >> n;

    std::vector<long long> v(10,0);
    int l = 0;
    for(int b = a; b != a || !l; ++l, b = (b * a) % 10) {
      ++v[b];
    }

    auto nn = n / l;
    auto nr = n % l;

    for(auto & i : v) {
      i *= nn;
    }

    for(int b = a, c = 0; c < (int)nr; ++c, b = (b * a) % 10) {
      ++v[b];
    }

    for(size_t i = 0, j = v.size(); i < j; ++i) {
      if(i) {
        std::cout << ", ";
      }
      std::cout << i << ": " << v[i];
    }

    std::cout << "\n";
  }
}
