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
  vector<char> r;
  for(auto val = std::stol(line); val;) {
    --val;
    r.push_back('A' + val % 26);
    val /= 26;
  }
  for(auto i = r.size(); i; --i) {
    std::cout << r[i - 1];
  }
  std::cout<<"\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "52",
    "3702"
  };
  vector<string> v_expect {
    "AZ",
    "ELJ"
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
