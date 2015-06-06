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
  size_t ofs = 0;
  while('0' != line[ofs] && '1' != line[ofs]) {
    ++ofs;
  }
  for(size_t i = 0; ' ' != line[i]; ++i, ++ofs) {
    auto mask = line[ofs];
    auto c = line[i];
    std::cout << (char)(('1' == line[ofs]) ? std::toupper(line[i]) : line[i]);
  }
  std::cout << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "hello 11001",
    "world 10000",
    "cba 111",
  };
  vector<string> v_expect {
    "HEllO",
    "World",
    "CBA",
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
