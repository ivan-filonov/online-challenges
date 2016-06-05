#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::cin.sync_with_stdio(false);

  int numTests;
  std::cin >> numTests;

  while (numTests-- > 0) {
    int t;
    std::string a, b;
    std::cin >> t >> a >> b;
    int d = 0;
    for (int i = 0; i < t; ++i) {
      if (a[i] != b[i]) {
        ++d;
      }
    }
    if (d % 2) {
      std::cout << "-1\n";
      continue;
    }

    int pos = 0;
    int res = 0;
    while (pos < t) {
      if (a[pos] == b[pos]) {
        ++pos;
        continue;
      }
      ++res;
      if (pos + 1 < t) {
        a[pos + 1] = ('0' + '1') - a[pos + 1];
      }
      ++pos;
    }
    std::cout << res << "\n";
  }

  return 0;
}
