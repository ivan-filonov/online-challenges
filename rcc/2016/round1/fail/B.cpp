#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

int main() {
  std::cin.sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (; t; --t) {
    using NUM = long long int;
    NUM n, tonnelLen;
    std::cin >> n >> tonnelLen;

    std::vector<NUM> a(n);
    std::vector<NUM> l(n);

    for (auto &k : a) {
      std::cin >> k;
    }
    for (auto &j : l) {
      std::cin >> j;
    }

    NUM cnt = 0;
    NUM tail = 0;
    for (size_t i = 0; i < a.size(); ++i) {
      if (l[i]) {
        tail = 0;
        continue;
      }
      tail += a[i];
      if (tail >= tonnelLen) {
        ++cnt;
        tail = 0;
      }
    }

    std::cout << cnt << "\n";
  }
}
