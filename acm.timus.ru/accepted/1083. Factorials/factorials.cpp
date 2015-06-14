#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int n;
  string ems;
  std::cin >> n >> ems;
  int k = ems.length();
  int r = 1;
  while(n > 0) {
    r *= n;
    n -= k;
  }
  std::cout << r << "\n";
  return 0;
}
