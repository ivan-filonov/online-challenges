#include <fstream>
#include <iostream>
#include <vector>

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
  using std::string;
  using std::vector;

  vector<string> v_dir {"UP", "DOWN", "RIGHT", "LEFT"};
  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    auto sc = line.find(';');
    auto sc2 = line.find(';', sc + 1);
    auto dir = line.substr(0, sc);
    auto dim = std::stoi(line.substr(sc + 1, sc2 - sc - 1));
    vector<int> board (dim * dim);
    auto pos = sc2 + 1;
    for(auto & dst : board) {
      size_t ofs;
      dst = std::stoi(line.substr(pos), &ofs);
      pos += ofs + 1;
    }
    // TODO: process
    //...
    for(int i = 0; i < dim * dim; ++i) {
      if(i) {
        std::cout << (i%dim ? ' ' :'|');
      }
      std::cout << board[i];
    }
    std::cout << "\n";
  }

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "RIGHT; 4; 4 0 2 0|0 0 0 8|4 0 2 4|2 4 2 2",
      "UP; 4; 2 0 2 0|0 2 0 4|2 8 0 8|0 8 0 16",
    };
    std::vector<std::string> v_expect {
      "0 0 4 2|0 0 0 8|0 4 2 4|0 2 4 4",
      "4 2 2 4|0 16 0 8|0 0 0 16|0 0 0 0",
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
}
