#include <iostream>
#include <map>
#include <sstream>

int main() {
#ifdef ONLINE_JUDGE
  using std::cin;
  std::cin.sync_with_stdio(false);
#else
  std::istringstream cin{"5\n7\n121\n123\n7\n121\n###\n4\n3\n3\n2\n5\n"};
#endif

  std::string temp;

  std::getline(cin, temp);
  const int num_values = std::stoi(temp);

  std::map<int, int> map;

  for (int i = 0; i < num_values; ++i) {
    std::getline(cin, temp);
    ++map[std::stoi(temp)];
  }
  int cur = 0;

  std::map<int, int> rmap;
  for (auto &pair : map) {
    cur += pair.second;
    pair.second = cur;
    rmap[cur] = pair.first;
  }

  std::getline(cin, temp);

  std::getline(cin, temp);
  int num_queries = std::stoi(temp);
  while (num_queries-- > 0) {
    std::getline(cin, temp);
    std::cout << rmap.lower_bound(std::stoi(temp))->second << "\n";
  }

  return 0;
}
