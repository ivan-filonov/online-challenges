#include <iostream>
#include <sstream>
#include <vector>

template <typename V> using vector = std::vector<V>;

int main() {
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin;
#endif
  return 0;
}
