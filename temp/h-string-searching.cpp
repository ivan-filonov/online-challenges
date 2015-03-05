#include <fstream>
#include <iostream>
#include <vector>

#define TEST

namespace {
#ifdef TEST
  void test();
#endif //#ifdef TEST
  void process_file(char*);
}

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  void process(std::string s);

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "Hello,ell",
      "This is good, is",
      "CodeEval,C*Eval",
      "CodeEval,C\\*Eval",
      "Old,Young",
    };
    std::vector<std::string> v_expect {
      "true",
      "true",
      "true",
      "false",
      "false",
    };
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      process(v_test[i]);
      std::cout << v_expect[i] << "\n";
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      process(line);
    }
  }

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  //    std::istringstream ss { line };
    const int comma_pos = line.find(',');
    std::string str = line.substr(0, comma_pos);
    std::string pattern = line.substr(comma_pos + 1);
    std::cout << "str = '" << str << "', pattern = '" << pattern << "'\n";
  }
}
