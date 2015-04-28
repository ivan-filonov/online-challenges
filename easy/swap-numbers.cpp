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
  size_t i = 0;
  while(line.length() != i) {
    while(line.length() != i && std::isspace(line[i])) {
      ++i;
    }
    auto j = i + 1;
    while(!std::isdigit(line[j])) {
      ++j;
    }
    std::swap(line[i], line[j]);
    i = j + 1;
  }
  std::cout << line << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "4Always0 5look8 4on9 7the2 4bright8 9side7 3of8 5life5",
    "5Nobody5 7expects3 5the4 6Spanish4 9inquisition0",
  };
  vector<string> v_expect {
    "0Always4 8look5 9on4 2the7 8bright4 7side9 8of3 5life5",
    "5Nobody5 3expects7 4the5 4Spanish6 0inquisition9",
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
