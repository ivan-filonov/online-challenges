#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int a, b, c;
  std::cin >> a >> b >> c;
  std::cout << (a * b * c * 2);
  return 0;
}
