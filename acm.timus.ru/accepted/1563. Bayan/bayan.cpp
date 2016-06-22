#include <iostream>
#include <sstream>
#include <unordered_set>
int main() {
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"12\nESPRIT\nNice Connection\nCamelot\nAdilisik\nLady "
                         "and Gentleman City\nMEXX\nCamelot\nSultanna "
                         "Frantsuzova\nCamaieu\nMEXX\nAxara\nCamelot"};
#endif
  std::string t;
  std::getline(cin, t);
  int n = std::stoi(t), res = 0;
  std::unordered_set<std::string> s;
  while (n-- > 0) {
    std::getline(cin, t);
    if (s.count(t)) {
      ++res;
    } else {
      s.insert(t);
    }
  }
  std::cout << res << "\n";
  return 0;
}
