#include <iostream>
#include <sstream>
#include <unordered_map>

template <typename K, typename V> using map = std::unordered_map<K, V>;

int main() {
#ifdef ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"6\nregister vasya 12345\nlogin vasya 1234\nlogin "
                         "vasya 12345\nlogin anakin C-3PO\nlogout "
                         "vasya\nlogout vasya"};
#endif
  int n;
  cin >> n;

  map<std::string, std::string> mp;
  map<std::string, int> ml;

  while (n-- > 0) {
    std::string cmd, uname, pass;
    cin >> cmd >> uname;
    if (cmd == "register") {
      cin >> pass;
      if (mp.count(uname)) {
        std::cout << "fail: user already exists\n";
      } else {
        mp[uname] = pass;
        std::cout << "success: new user added\n";
      }
    } else if (cmd == "login") {
      cin >> pass;
      if (!mp.count(uname)) {
        std::cout << "fail: no such user\n";
      } else if (pass != mp[uname]) {
        std::cout << "fail: incorrect password\n";
      } else if (ml[uname]) {
        std::cout << "fail: already logged in\n";
      } else {
        std::cout << "success: user logged in\n";
        ml[uname] = 1;
      }
    } else if (cmd == "logout") {
      if (!mp.count(uname)) {
        std::cout << "fail: no such user\n";
      } else if (ml[uname]) {
        ml[uname] = 0;
        std::cout << "success: user logged out\n";
      } else {
        std::cout << "fail: already logged out\n";
      }
    }
  }
  return 0;
}
