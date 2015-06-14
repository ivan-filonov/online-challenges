#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int n;
  std::cin >> n;
  if(!n) {
    n = 1;
  } else if (n > 0) {
    n = n * (n + 1) / 2;
  } else {
    n = 1 - n * (n - 1) / 2;
  }
  std::cout << n << "\n";
  return 0;
}
