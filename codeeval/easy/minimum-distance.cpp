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
      R"raw(4 3 3 5 7)raw",
      R"raw(3 20 30 40)raw"
    };
    std::vector<std::string> v_expect {
      R"raw(6)raw",
      R"raw(20)raw"
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
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    std::vector<int> h;
    std::istringstream ss { line };
    int hb = -1, hf = -1;
    bool mid = false;
    for(std::string t; std::getline(ss, t, ' ');) {
      if(!mid) {
        mid = true;
        continue;
      }
      const auto val = std::stoi(t);
      if(-1 == hb || hb < val) {
        hb = val;
      }
      if(-1 == hf || hf > val) {
        hf = val;
      }
      h.push_back(val);
    }

    std::vector<int> d(hb + 1 - hf, 0);
    for(auto i : h) {
      for(int j = 0; j <= hb-hf; ++j) {
        d[j] += std::abs(i-j-hf);
      }
    }

    std::sort(d.begin(), d.end());
    std::cout << d.front() << "\n";
  }
}
