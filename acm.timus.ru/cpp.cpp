#include <iostream>
#include <sstream>
#include <vector>

using std::string;
template <typename V> using vector = std::vector<V>;

int main() {
#ifdef ONLINE_JUDGE
  using std::cin;
  std::cin.sync_with_stdio(false);
#else
  std::istringstream cin;
#endif
  return 0;
}
