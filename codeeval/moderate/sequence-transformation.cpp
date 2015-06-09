#include <fstream>
#include <iostream>
#include <regex.h>
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
  void process(std::string s);

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "1010 AAAAABBBBAAAA",
      "00 AAAAAA",
      "01001110 AAAABAAABBBBBBAAAAAAA",
      "1100110 BBAABABBA",
    };
    std::vector<std::string> v_expect {
      "Yes",
      "Yes",
      "Yes",
      "No",
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
    auto sp = line.find(' ');
    std::string rs;
    for(auto c : line.substr(0, sp)) {
      if('0' == c) {
        rs += "(A+)";
      } else {
        rs += "(A+|B+)";
      }
    }
    regex_t rx;
    regcomp(&rx, rs.c_str(), REG_EXTENDED);
    regmatch_t rm;
    bool flag = !regexec(&rx, line.substr(sp+1).c_str(), 1, &rm, 0);
    if(flag) {
      flag = sp + 1 + rm.rm_eo - rm.rm_so == line.length();
    }
    std::cout << (flag ? "Yes\n" : "No\n"); 
  }
}
