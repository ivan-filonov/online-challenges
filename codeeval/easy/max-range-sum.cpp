#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

//#define TEST

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
      "5;7 -3 -10 4 2 8 -2 4 -5 -2",
      "6;-4 3 -10 5 3 -7 -3 7 -6 3",
      "3;-7 0 -45 34 -24 7",
    };
    std::vector<std::string> v_expect {
      "16",
      "0",
      "17",
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
    std::string t;
    std::getline(ss, t, ';');
    const int days = std::stoi(t);
    std::vector<int> v;
    while(std::getline(ss, t, ' ')) {
      v.push_back(std::stoi(t));
    }
    int sum = 0;
    for(int i = 0; i < days; ++i) {
      sum += v[i];
    }
    int msum = sum;
    for(int i = days; i < v.size(); ++i) {
      sum += v[i] - v[i-days];
      msum = std::max(sum, msum);
    }
    msum = std::max(msum, 0);
    std::cout << msum << "\n";
  }
}
