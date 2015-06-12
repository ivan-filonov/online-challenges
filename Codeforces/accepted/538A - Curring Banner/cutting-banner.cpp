#include <iostream>
std::string cf { "CODEFORCES" };
int main() {
  std::string s;
  std::cin >> s;
  size_t p, r;
  for(p = 0; p != s.length() && p != cf.length() && s[p] == cf[p]; ++p);
  for(r = 0; r != s.length() && r != cf.length() && s[s.length() - 1 - r] == cf[cf.length() - 1 - r]; ++r);
  std::cout << ((s.length() >= cf.length() && p + r >= cf.length()) ? "YES" : "NO");
  return 0;
}

