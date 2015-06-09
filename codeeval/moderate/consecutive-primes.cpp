#include <fstream>
#include <iostream>
#include <unordered_set>
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
  int process(std::string s);
  void init();

#ifdef TEST
  void test() {
    init();
    std::vector<std::string> v_test {
      "2",
      "4",
      "18",
    };
    std::vector<std::string> v_expect {
      "1",
      "2",
      "0",
    };
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      std::cout << process(v_test[i]) << "\n";
      std::cout << v_expect[i] << "\n";
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    init();
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      std::cout << process(line) << "\n";
    }
  }

  using std::vector;
  using std::unordered_set;

  vector<int> primes(40,0);

  int test(vector<int> &v, int N, vector<int> &used) {
    if(v.size() == N) {
      return primes[v.back() + v.front()];
    }
    
    int res = 0;
    for(int i = 1; i <= N; ++i) {
      if(used[i]) {
        continue;
      }
      if(!primes[v.back() + i]) {
        continue;
      }
      used[i] = 1;
      v.push_back(i);
      res += test(v, N, used);
      v.pop_back();
      used[i] = 0;
    }
    return res;
  }

  int process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  //    std::istringstream ss { line };
    const int num = std::stoi(line);
    if(num % 2) {
      return 0;
    }

    vector<int> v(1,1);
    vector<int> used(num + 1, 0);
    used[1] = 1;

    return test(v, num, used);
  }

  void init() {
    primes[2] = 1;
    primes[3] = 1;
    for(int i = 1; i < 7; ++i) {
      for(int d = -1; d < 2; d += 2) {
        const int num = i * 6 + d;
        bool flag = true;
        for(int j = 0; j < primes.size(); ++j) {
          if(primes[j] && 0 == num % j) {
            flag = false;
            break;
          }
        }
        if(flag) {
          primes[num] = 1;
        }
      }
    }
  }
}
