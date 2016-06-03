#include <cmath>
#include <iostream>
#include <sstream>
#include <unordered_map>

int f(const int val);

std::unordered_map<int, int> m{{0, 0}, {1, 1}};
int hits = 0, misses = 0;
int mf(const int val) {
  if (!val) {
    return 0;
  }
  auto it = m.find(val);
  if (it == m.end()) {
    const int res = f(val);
    m.emplace(val, res);
    ++misses;
    return res;
  } else {
    ++hits;
    return it->second;
  }
}

int f(const int val) {
  int s = std::sqrt(val);
  int res = val;
  for (int i = 1, j = 1; i <= s; j += 2 * i + 1, i += 1) {
    const int cur = val / j + mf(val % j);
    res = std::min(res, cur);
  }
  return res;
}

int main() {
#ifdef ONLINE_JUDGE
  int value;
  std::cin >> value;
#else
  int value = 41;
#endif

  std::cout << mf(value) << "\n";

  return 0;
}
