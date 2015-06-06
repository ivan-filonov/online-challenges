#include <algorithm>
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

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  vector<char> buf(line.length());
  std::copy(line.begin(), line.end(), buf.begin());
  if(std::next_permutation(buf.begin(), buf.end())) {
    for(char c : buf) {
      std::cout << c;
    }
  } else {
    auto z = std::count(buf.begin(), buf.end(), '0') + 1;
    bool mid = false;
    for(char c : buf) {
      if('0' == c) {
        continue;
      }
        std::cout << c;
      if(!mid) {
        mid = true;
        while(z--) {
          std::cout << '0';
        }
      }
    }
  }
  std::cout << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "115",
    "842",
    "8000",
    "511",
    "1015",
    "1051",
  };
  vector<string> v_expect {
    "151",
    "2048",
    "80000",
    "?",
    "?",
    "?",
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
