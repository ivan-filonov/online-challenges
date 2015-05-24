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

const vector<string> numbers {
  "",
  "One",
  "Two",
  "Three",
  "Four",
  "Five",
  "Six",
  "Seven",
  "Eight",
  "Nine",
  "Ten",
  "Eleven",
  "Twelve",
  "Thirteen",
  "Fourteen",
  "Fifteen",
  "Sixteen",
  "Seventeen",
  "Eighteen",
  "Nineteen"
};

const vector<string> n10 {
  "Twenty",
  "Thirty",
  "Forty",
  "Fifty",
  "Sixty",
  "Seventy",
  "Eighty",
  "Ninety",
};

void sub1k(int v) {
  if(v >= 100) {
    std::cout << numbers[v/100] << "Hundred";
    v %= 100;
  }
  if(v < 20) {
    std::cout << numbers[v];
  } else {
    std::cout << n10[(v - 20)/10];
    v %= 10;
    if(v) {
      std::cout << numbers[v];
    }
  }
}

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
//    std::istringstream ss { line };
  auto v = std::stol(line);
  const auto c_1e6 = 1000000;
  const auto c_1e3 = 1000;
  if(v) {
    if(v >= c_1e6) {
      sub1k(v / c_1e6);
      std::cout << "Million";
      v %= c_1e6;
    }
    if(v >= c_1e3) {
      sub1k(v / c_1e3);
      std::cout << "Thousand";
      v %= c_1e3;
    }
    sub1k(v);
  } else {
    std::cout << "Zero";
  }
  std::cout << "Dollars\n";
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
