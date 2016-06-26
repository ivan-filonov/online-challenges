#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

int main() {
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"6 1 2 2 3 3 3"};
#endif
  int n, i;
  cin >> n;
  std::vector<int> v;
  std::set<std::vector<int>> sv;
  while (n-- > 0) {
    cin >> i;
    v.push_back(i);
  }
  std::sort(v.begin(), v.end());
  for(int i = 0; i < 6; ++i) {
		std::next_permutation(v.begin(), v.end());
    sv.insert(v);
    if (sv.size() >= 6) {
      break;
    }
  }
  std::cout << (sv.size() >= 6 ? "Yes\n" : "No\n");
  return 0;
}
