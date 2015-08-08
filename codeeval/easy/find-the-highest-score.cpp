#include <fstream>
#include <iostream>
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
// required when TEST is defined
template<typename V> using vector = std::vector<V>;

void process(string line) {
#ifdef TEST
	std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
	std::istringstream ss { line };

	int pos = 0;
	vector<int> v;

	for(string s; ss >> s;) {
		if(s == "|") {
			pos = 0;
			continue;
		}
		auto i = std::stoi(s);
		if(v.size() == pos) {
			v.resize(pos + 1);
			v[pos] = i;
		} else {
			v[pos] = std::max(i, v[pos]);
		}
		++pos;
	}

	bool mid = false;
	for(int i : v) {
		std::cout << (mid ? " " : "") << i;
		mid = true;
	}
	std::cout << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
	"72 64 150 | 100 18 33 | 13 250 -6",
	"10 25 -30 44 | 5 16 70 8 | 13 1 31 12",
	"100 6 300 20 10 | 5 200 6 9 500 | 1 10 3 400 143",
  };
  vector<string> v_expect {
	"100 250 150",
	"13 25 70 44",
	"100 200 300 400 500",
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
