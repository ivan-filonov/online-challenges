#include <iostream>
int main() {
  int w;
  std::cin >> w;
  std::cout << (((w % 2) || (w < 4)) ? "NO" : "YES");
  return 0;
}
