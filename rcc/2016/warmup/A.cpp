#include <cstdio>
#include <iostream>
int main() {
  std::string scode;
  std::getline(std::cin, scode);

  std::string t;
  std::getline(std::cin, t);

  for (int i = 0, e = std::stoi(t); i < e; ++i) {
    std::getline(std::cin, t);
    int a{0};
    int b{0};
    for (size_t idx = 0; idx < t.length(); ++idx) {
      if (idx < scode.length() && scode[idx] == t[idx]) {
        ++a;
        continue;
      }
      if (scode.find(t[idx]) != scode.npos) {
        ++b;
      }
    }
    std::cout << a << " " << b << "\n";
  }
}
