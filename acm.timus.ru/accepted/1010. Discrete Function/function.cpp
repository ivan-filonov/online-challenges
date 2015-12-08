#include <iostream>
#include <vector>

using std::string;
template <typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int n;
  std::cin >> n;
  long long a, b, d, d2;
  n -= 2;
  std::cin >> a >> b;
  d = std::abs(a - b);
  int ai = 1, ri = 1;
  while (n-- > 0) {
    a = b;
    std::cin >> b;
    d2 = std::abs(a - b);
    ++ai;

    if (d2 > d) {
      ri = ai;
      d = d2;
    }
  }
  std::cout << ri << ' ' << (ri + 1);
  return 0;
}
