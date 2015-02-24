#include <iostream>

//#define CPP_REGEX

#ifdef CPP_REGEX
#include <regex>
#else//#ifdef CPP_REGEX
#include <regex.h>
#endif//#ifdef CPP_REGEX

int main() {
  std::string ss[] = { "a.a", "abc.a", "abc.a.b" , "b.b"  };
#ifdef CPP_REGEX
  std::regex r1 (".*\\.a");
  for(auto & s : ss) {
    std::cout << s << "->" << std::regex_match(s, r1) << "\n";
  }
  std::cout << std::string(60,'-') << "\n";
  r1.assign("[^ ]+\\.b");
  for(auto & s : ss) {
    std::cout << s << "->" << std::regex_match(s, r1) << "\n";
  }
#else//#ifdef CPP_REGEX
  regex_t rx;
  const std::string rxs { R"(.*\.a)" };
  if(regcomp(&rx, rxs.c_str(), REG_EXTENDED)) {
    std::cout << "bad regex: " << rxs << "\n";
    return 1;
  }

  for(auto &s : ss) {
    regmatch_t m;
    if(!regexec(&rx, s.c_str(), 1, &m, 0)) {
      std::cout << s << "->" << s.substr(m.rm_so, m.rm_eo-m.rm_so) << "\n";
    }
  }
#endif//#ifdef CPP_REGEX
  return 0;
}

