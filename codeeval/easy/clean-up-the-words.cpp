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
  bool spc = false, mid = false;
  for(auto c : line) {
	  if(std::isalpha(c)) {
		  if(spc) {
			  if(mid) {
				  std::cout << ' ';
			  }
			  spc = false;
		  }
		  std::cout << (char)std::tolower(c);
		  mid = true;
	  } else {
		  spc = true;
	  }
  }
  std::cout << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
	"(--9Hello----World...--)",
	"Can 0$9 ---you~",
	"13What213are;11you-123+138doing7",
  };
  vector<string> v_expect {
	"hello world",
	"can you",
	"what are you doing",
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
