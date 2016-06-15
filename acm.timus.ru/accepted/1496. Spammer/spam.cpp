#include <iostream>
#include <sstream>
#include <unordered_map>

int main() {
#ifdef ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"11\nnaucoder\niceman\nabikbaev\nabikbaev\npetr\nabikb"
                         "aev\nabikbaev\nx\nabikbaev\nacrush\nx\n"};
#endif

  std::unordered_map<std::string, int> m;
  int n;
  cin >> n;

  for (std::string t; cin >> t && n > 0; --n) {
    m[t] += 1;
  }

  for (auto &p : m) {
    if (p.second > 1) {
      std::cout << p.first << "\n";
    }
  }

  return 0;
}
