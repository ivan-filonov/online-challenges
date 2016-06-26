#include <iostream>
#include <sstream>
#include <unordered_set>

int main() {
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"Gateway\n"
                         "Machinery-Gateway\n"
                         "Machinery-Control\n"
                         "Control-Central\n"
                         "Control-Engine\n"
                         "Central-Engine\n"
                         "Storage-Gateway\n"
                         "Storage-Waste\n"
                         "Central-Waste\n"
                         "#"};
#endif
  std::unordered_set<std::string> names;
  for (std::string t; cin >> t && t != "#";) {
    if (names.empty()) {
      names.insert(t);
      continue;
    }
    auto mid = t.find('-');
    names.insert(t.substr(0, mid));
    names.insert(t.substr(mid + 1));
  }
  std::cout << (names.empty() ? 0 : names.size() - 1) << "\n";

  return 0;
}
