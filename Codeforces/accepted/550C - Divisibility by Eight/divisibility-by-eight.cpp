#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::string s;
  std::cin >> s;
  
  if(std::string::npos != s.find_first_of("08")) {
    std::cout << "YES\n" << s[s.find_first_of("08")];
    return 0;
  }

  for(auto & c : s) {
    c -= '0';
  }
  std::reverse(s.begin(), s.end());
  bool flag = false;
 
  for(int i = 0; i != s.length(); ++i) {
    for(int j = i + 1; j != s.length(); ++j) {
      if(s[j] && (s[i] + s[j] * 10) % 8 == 0) {
        std::cout << "YES\n" << (char)('0' + s[j]) << (char)('0' + s[i]);
        return 0;
      }
    }
  }
  
  for(int i = 0;  i != s.length(); ++i) {
    for(int j = i + 1; j != s.length(); ++j) {
      for(int k = j + 1; k != s.length(); ++k) {
        if(s[k] && (s[i] + s[j] * 10 + s[k] * 100) % 8 == 0) {
          std::cout << "YES\n" << (char)('0' + s[k]) << (char)('0' + s[j]) << (char)('0' + s[i]);
          return 0;
        }
      }
    }
  }

  std::cout << "NO";
  return 0;
}
