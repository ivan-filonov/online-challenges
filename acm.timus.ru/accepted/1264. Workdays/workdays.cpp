#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  std::cout << (n * (m + 1));
  return 0;
}
