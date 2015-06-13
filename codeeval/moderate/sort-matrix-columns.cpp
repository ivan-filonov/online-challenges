#include <fstream>
#include <iostream>
#include <vector>

#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char*);

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

using std::string;
// required when TEST is defined
template<typename V> using vector = std::vector<V>;

int ccomp(vector<vector<int>> & m, int c0, int c1) {
  for(auto & l : m) {
    if(l[c0] < l[c1]) {
      return -1;
    }
    if(l[c0] > l[c1]) {
      return 1;
    }
  }
  return 0;
}

void cswap(vector<vector<int>> & m, int c0, int c1) {
  for(auto & l : m) {
    std::swap(l[c0], l[c1]);
  }
}

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  vector<vector<int>> m(1);
  int val = 0, sign = 0;
  for(char c : line) {
    if(' ' == c) {
      if(sign) {
        m.back().push_back(val * sign);
        sign = 0;
        val = 0;
      }
    } else if('|' == c) {
      m.emplace_back();
    } else if('-' == c) {
      sign = -1;
    } else if(std::isdigit(c)) {
      if(!sign) {
        sign = 1;
      }
      val = val * 10 + (c - '0');
    }
  }
  if(sign) {
    m.back().push_back(val * sign);
  }

  for(int i = 0; i != m[0].size(); ++i) {
    for(int j = i + 1; j != m[0].size(); ++j) {
      if(ccomp(m, i, j) > 0) {
        cswap(m, i, j);
      }
    }
  }

  bool m0 = false;
  for(auto & l : m) {
    std::cout << (m0 ? " | " : "");
    m0 = true;
    bool m1 = false;
    for(auto i : l) {
      std::cout << (m1 ? " " : "") << i;
      m1 = true;
    }
  }
  std::cout << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "-3 29 -3 | -17 69 -17 | 44 3 8",
    "25 39 -26 -21 | -81 -98 -91 27 | 32 -87 67 98 | -90 -79 18 9",
    "26 -10 39 | -62 66 97 | 22 85 36",
  };
  vector<string> v_expect {
    "-3 -3 29 | -17 -17 69 | 8 44 3",
    "-26 -21 25 39 | -91 27 -81 -98 | 67 98 32 -87 | 18 9 -90 -79",
    "-10 26 39 | 66 -62 97 | 85 22 36",
  };
  for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    process(v_test[i]);
    std::cout << v_expect[i] << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    process(line);
  }
}
