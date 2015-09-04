#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char *);

int main(int argc, char **argv) {
#ifdef TEST
  test();
#else  //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
  return 0;
}

using std::string;
// required when TEST is defined
template <typename V> using vector = std::vector<V>;

bool haveAll(const string & str, const string & letters) {
	char sl[256] = { 0, };
	char ll[256] = { 0, };
	for(auto c : str) {
		sl[c]++;
	}
	for(auto c : letters) {
		ll[c]++;
	}
	for(int i = 0; i < 256; ++i) {
		if(sl[i] < ll[i]) {
			return false;
		}
	}
	return true;
}

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  std::istringstream ss{line};

  vector<string> v;
  for (string t; ss >> t && t != "|";) {
    v.emplace_back(std::move(t));
  }

  string pat;
  ss >> pat;

  bool mid = false;
  for(auto & s : v) {
	  if(!haveAll(s, pat)) {
		  continue;
	  }
	  std::cout << (mid ? " " :"") << s;
	  mid = true;
  }
  if(!mid) {
	  std::cout << "False";
  }
  std::cout << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test{
      "Cabernet Merlot Noir | ot", "Chardonnay Sauvignon | ann",
      "Shiraz Grenache | o",
  };
  vector<string> v_expect{
      "Merlot", "Chardonnay Sauvignon", "False",
  };
  for (int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    process(v_test[i]);
    std::cout << v_expect[i] << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char *path) {
  std::ifstream stream(path);
  for (string line; std::getline(stream, line);) {
    process(line);
  }
}
