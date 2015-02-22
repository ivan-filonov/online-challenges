#include <iostream>
#include <regex>

int main() {
  std::string ss[] = { "a.a", "abc.a", "abc.a.b" , "b.b"  };
  std::regex r1 (".*\\.a");
  for(auto & s : ss) {
    std::cout << s << "->" << std::regex_match(s, r1) << "\n";
  }
  std::cout << std::string(60,'-') << "\n";
  r1.assign("[^ ]+\\.b");
  for(auto & s : ss) {
    std::cout << s << "->" << std::regex_match(s, r1) << "\n";
  }
  return 0;
}

