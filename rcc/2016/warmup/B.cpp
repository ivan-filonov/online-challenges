#include <iostream>
#include <map>

int main() {
  int n;
  std::cin >> n;

  std::map<int, int> map;
  for (int i = 0; i < n; ++i) {
    int j;
    std::cin >> j;
    map[j]++;
  }

  auto it = map.rbegin();
  int v1 = it->first;
  if (it->second < 2) {
    ++it;
  }
  int v2 = it->first;

  int res = (v1 + v2) / 2;

  std::cout << res << "\n";
}
