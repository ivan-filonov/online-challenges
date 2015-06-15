#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

vector<string> t {
  "APOR",
  "BMS",
  "DGJKTW",
};

int lookup(string s) {
  int i;
  for(i = 0; i != t.size(); ++i) {
    if(string::npos != t[i].find(s[0])) {
      break;
    }
  }
  return i;
}

int main() {
  std::cin.sync_with_stdio(false);

  int n;
  std::cin >> n;

  string name;
  int steps = 0, pos = 0, next_pos;

  while(n-->0) {
    std::cin >> name;
    next_pos = lookup(name);
    steps += std::abs(pos - next_pos);
    pos = next_pos;
  }

  std::cout << steps;

  return 0;
}
