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

	string t;
	ss >> t;

	auto edge = t.length();
	vector<int> m;
	for(int i = 0; i < edge; ++i) {
		for(auto c : t) {
			m.push_back(c - '0');
		}
		ss >> t;
		ss >> t;
	}

	vector<int> v = m;
	int ms = 1;
	int cc = v[0];

	while(ms < edge) {
		bool fail = false;
		for(int i = 0, ii = 0; i < edge - ms + 1; ++i, ii += edge) {
			for(int j = 0; j < edge - ms + 1; ++j) {
				if(v[ii + j] != cc) {
					fail = true;
					break;
				}
			}
			if(fail) {
				break;
			}
		}
		if(!fail) {
			break;
		}

		for(int i = 0, ii = 0; i < edge - ms; ++i, ii += edge) {
			for(int j = 0; j < edge - ms; ++j) {
				int ofs = ii + j;
				for(int k = 0; k < ms; ++k) {
					v[ofs] += m[ofs + ms + k * edge];
					v[ofs] += m[ofs + ms * edge + k];
				}
				v[ofs] += m[ofs + ms * edge + ms];
			}
		}
		cc = v[0];
		++ms;
	}
	std::cout << ms << "x" << ms << ", " << cc << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
	"11 | 11",
	"1001 | 0110 | 1001 | 0110",
	"110 | 101 | 111",
	"000 | 000 | 000",
  };
  vector<string> v_expect {
	"1x1, 1",
	"2x2, 2",
	"3x3, 7",
	"1x1, 0",
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
