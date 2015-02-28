#include <fstream>
#include <iostream>
#include <vector>

namespace {
  void test();
  void process_file(char*);
}

int main(int argc, char ** argv) {
    if( 1 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

namespace {
  void process(std::string s);

  void test() {
    std::vector<std::string> v_test {};
    std::vector<std::string> v_expect {};
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      process(v_test[i]);
      std::cout << v_expect[i] << "\n";
    }
  }

  void process_file(char* path) {
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      process(line);
    }
  }

  void process(std::string line) {
    std::cout << "s = '" << line << "'\n";
  //    std::istringstream ss { line };
  }
}
