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
  vector<int> a(1, 1), b(1, 2), c;
  for(int n = 1, nn = std::stoi(line); n != nn; ++n) {
    c.clear();
    int carry = 0;
    for(size_t i = 0, j = std::max(a.size(), b.size()); i != j; ++i) {
      int _a = 0, _b = 0;
      if(i < a.size()) _a = a[i];
      if(i < b.size()) _b = b[i];
      int s = _a + _b + carry;
      carry = s / 10000;
      c.push_back(s % 10000);
    }
    if(carry) {
      c.push_back(carry);
    }
    a.swap(b);
    b.swap(c);
  }

  while(!a.back()) {
    a.pop_back();
  }

  std::cout << a.back();
  a.pop_back();
  while(!a.empty()) {
    int x = a.back();
    a.pop_back();
    if(x < 1000) std::cout << '0';
    if(x < 100) std::cout << '0';
    if(x < 10) std::cout << '0';
    std::cout << x;
  }
  std::cout << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "10", "20"
  };
  vector<string> v_expect {
    "89", "10946"
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
