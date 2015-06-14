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
  string word, sub;
  size_t delim = line.find(',');
  word = line.substr(0, delim);
  sub = line.substr(delim + 1);

  vector<string> pref;
  for(char c : word) {
    vector<string> temp;
    for(auto & p : pref) {
      if(p.length() == sub.length()) {
        continue;
      }
      if(c == sub[p.length()]) {
        temp.emplace_back(p + c);
      }
    }
    for(auto & t : temp) {
      pref.push_back(std::move(t));
    }
    if(c == sub[0]) {
      pref.emplace_back(string() + c);
    }
  }

  int c = 0;
  for(auto & p : pref) {
    if(p.length() == sub.length()) {
      ++c;
    }
  }

  std::cout << c << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "babgbag,bag",
    "rabbbit,rabbit",
  };
  vector<string> v_expect {
    "5", "3"
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
