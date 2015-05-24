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
template<typename V> using vector = std::vector<V>;

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
//    std::istringstream ss { line };
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "3",
    "10",
    "21",
    "466",
    "1234",
  };
  vector<string> v_expect {
    "ThreeDollars",
    "TenDollars",
    "TwentyOneDollars",
    "FourHundredSixtySixDollars",
    "OneThousandTwoHundredThirtyFourDollars",
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
