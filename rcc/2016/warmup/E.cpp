#include <iostream>
#include <vector>
int main() {
  using N = long long int;
  N n, m;
  std::vector<N> a, b;
  std::cin >> n >> m;
  a.resize(n);
  b.resize(m);
  for (auto &v : a)
    std::cin >> v;
  for (auto &v : b)
    std::cin >> v;
  int x = 0;
  for (auto z : b) {
    N res = 0;
    N cur = 0;
    for (auto v : a) {
      if (v < 0 && !cur)
        continue;
      if (z > v) {
        cur = 0;
        continue;
      }
      cur += v;
      if (cur < 0) {
        cur = 0;
        continue;
      }
      if (cur > res)
        res = cur;
    }
    if (x)
      std::cout << ' ';
    x = 1;
    std::cout << res;
  }
  std::cout << "\n";
}
