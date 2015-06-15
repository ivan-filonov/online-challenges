#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  
  int a, b;
  std::cin >> a >> b;

  std::cout << (b - 1) << " " << (a - 1);

  return 0;
}
