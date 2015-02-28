#include <fstream>
#include <iostream>

//#define CPP_REGEX
#ifdef CPP_REGEX
#include <regex>
#else//#ifdef CPP_REGEX
#include <regex.h>
#endif//#ifdef CPP_REGEX

namespace {
  void test();
  void process_file(char*);
}

int main(int argc, char ** argv) {
    if( 0 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

namespace {
  void process(std::string s);

  void test() {
    process("foo@bar.com");
    std::cout << "true"    "\n";
    process("this is not an email id");
    std::cout << "false"    "\n";
    process("admin#codeeval.com");
    std::cout << "false"    "\n";
    process("good123@bad.com");
    std::cout << "true"    "\n";
  }

  void process_file(char* path) {
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      process(line);
    }
  }

  const std::string re_str { R"([a-zA-Z][a-zA-Z0-9_.+-]*@[a-zA-Z0-9][a-zA-Z0-9-]*(\.[a-zA-Z0-9][a-zA-Z0-9.-]*)*$)" };
#ifdef CPP_REGEX
  std::regex rx { re_str };
#else//#ifdef CPP_REGEX
  regex_t rx;
  int rx_comp = regcomp(&rx, re_str.c_str(), REG_EXTENDED);
#endif//#ifdef CPP_REGEX

  void process(std::string line) {
#ifdef CPP_REGEX
    std::smatch sm;
    std::cout << ((std::regex_match(line, sm, rx) && sm.str(0) == line)?"true\n":"false\n");
#else//#ifdef CPP_REGEX
    regmatch_t m;
    std::cout << ((!regexec(&rx, line.c_str(), 1, &m, 0) && !m.rm_so && m.rm_eo == line.length())?"true\n":"false\n");
#endif//#ifdef CPP_REGEX
  }
}
