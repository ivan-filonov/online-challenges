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
      "10 100",
      "8382 8841",
    };
    std::vector<std::string> v_expect {
      "13 15 17 19 31 35 37 39 51 53 57 59 71 73 75 79 91 93 95 97",
      "-1",
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
    size_t sep;
    auto A = std::stoi(line, &sep);
    auto B = std::stoi(line.substr(sep));
    bool mid = false;
    for(auto n = A; n <= B; ++n) {
      auto s = std::to_string(n);
      int mask = 0;
      int pos = 0;
      for(int i = 0; i < s.length(); ++i) {
        pos += s[pos] - '0';
        pos %= s.length();
        mask |= 1 << pos;
      }
      ++mask;
      if(mask == (1 << s.length())) {
        bool dups = false;
        int mask2 = 0;
        for(auto c : s) {
          int b = 1 << (c - '0');
          if(mask2 & b) {
            dups = true;
            break;
          }
          mask2 |= b;
        }
        if(dups) {
          continue;
        }
        if(mid) {
          std::cout << " ";
        } else {
          mid = true;
        }
        std::cout << n;
      }
    }
    std::cout << (mid ? "\n" : "-1\n");
  }
}
