#include <algorithm>
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int k;
  std::cin >> k;
  vector<int> v;
  v.resize(k);
  for(int & i : v) {
    std::cin >> i;
  }
  std::sort(v.begin(), v.end());
  int res = 0;
  for(int group_id = 0; group_id < k/2 + 1; ++group_id) {
    res += v[group_id] / 2 + 1;
  }
  std::cout << res << "\n";
  return 0;
}
