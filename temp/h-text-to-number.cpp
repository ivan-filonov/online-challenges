#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
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
template<typename K, typename V> using map = std::map<K,V>;

string S_NEG { "negative" };
map<string,int> words {
  { "zero", 0 },  { "one", 1 },  { "two", 2 },  { "three", 3 },  { "four", 4 },  { "five", 5 },  { "six",  6 },  { "seven",  7 },  { "eight",  8 },  { "nine",  9 },
  { "ten",  10 },  { "eleven",  11 },  { "twelve", 12 },  { "thirteen",  13 },  { "fourteen",  14 },  { "fifteen",  15 },  { "sixteen",  16 },  { "seventeen",  17 },  { "eighteen",  18 },  { "nineteen",  19 },
  { "twenty", 20 },  { "thirty",  30 },  { "forty", 40 },  { "fifty", 50 },  { "sixty", 60 },  { "seventy", 70 },  { "eighty", 80 },  { "ninety",  90 },
  { "hundred", 100 },  { "thousand",  1000 },  { "million", 1000000 },
};

int process(string line) {
  vector<int> va;
  int sign = 1;
  bool mid = false;
  std::istringstream ss { line };
  for(string w; ss >> w; ) {
    if(!mid) {
      mid = true;
      if(w == S_NEG) {
        sign = -1;
        continue;
      }
    }
    if(!words.count(w)) {
      throw w;
    }
    auto v = words[w];
    if(va.empty()) {
      va.push_back(v);
    } else {
      if((100 == v && va.back() < 10) || (v >= 1000 && va.back() < 1000)) {
        va.back() *= v;
      } else if(v < 100 & va.back() < 1000) {
        va.back() += v;
      } else {
        va.push_back(v);
      }
    }
  }

  int res = 0;
  for(int n : va) res += n;

  return sign * res;
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "fifteen",
    "negative six hundred thirty eight",
    "zero",
    "two million one hundred seven",
    "negative five hundred sixty seven million eight hundred ninety thousand one hundred twenty three",
  };
  vector<string> v_expect {
    "15",
    "-638",
    "0",
    "2000107",
    "-567890123",
  };
  for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    std::cout << process(v_test[i]) << "\n";
    std::cout << v_expect[i] << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    std::cout << process(line) << "\n";
  }
}
