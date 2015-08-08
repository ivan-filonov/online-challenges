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
  std::string word;
  std::istringstream ss { line };
  for(std::string t; ss >> t;) {
	  if(t.length() > word.length()) {
		  word = t;
	  }
  }
  for(int i = 0; i != word.length(); ++i) {
	  if(i) {
		  std::cout << ' ';
		  for(int j = 0; j !=i; ++j) {
			  std::cout << '*';
		  }
	  }
	  std::cout << word[i];
  }
  std::cout << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
	"cat dog hello",
	"stop football play",
	"music is my life",
  };
  vector<string> v_expect {
	"h *e **l ***l ****o",
	"f *o **o ***t ****b *****a ******l *******l",
	"m *u **s ***i ****c",
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
