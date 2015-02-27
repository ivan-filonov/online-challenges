#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
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
#else
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  void process(std::string s);

/*
 *    m[0] = a
 *    m[i] = (b * m[i - 1] + c) % r, 0 < i < k 
 * */

#ifdef TEST
  void test() {
    // n,k,a,b,c,r
    std::vector<std::string> v_test {
      "78,51,3,5,5,51230",
      "186,75,68,16,539,312",
      "137,135,48,17,461,512",
      "98,22,6,30,524,100",
      "46,18,7,11,9,46",
    };
    std::vector<std::string> v_expect {
      "26",
      "34",
      "1",
      "6",
      "12",
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
    int n,k,a,b,c,r;
    char t;
    std::istringstream ss { line };
    ss >> n >> t >> k >> t >> a >> t >> b >> t >> c >> t >> r;
//    std::cout << "n = " << n << ", k = " << k << ", a = " << a << ", b = " << b << ", c = " << c << ", r = " << r << "\n";
    std::vector<int> v(n);
    v[0] = a;
    std::unordered_map<int,int> mm;
    mm[a]++;
    for(int i = 1; i < k; ++i) {
      v[i] = (v[i-1]*b+c)%r;
      mm[v[i]]++;
    }
    for(int i = k; i < n; ++i) {
      int j = 0;
      while(mm[j] > 0) {
        ++j;
      }
      v[i] = j;
      mm[j]++;
      mm[v[i-k]]--;
    }
    std::cout << v.back() << "\n";
  }
}
