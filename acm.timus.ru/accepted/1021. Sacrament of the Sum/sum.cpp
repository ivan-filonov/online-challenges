#include <iostream>
#include <vector>

using std::string;
template <typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int n;
  std::cin >> n;
  vector<int> asc(n);
  for (auto &i : asc)
    std::cin >> i;

  std::cin >> n;
  vector<int> dsc(n);
  for (auto &i : dsc)
    std::cin >> i;

  auto ai = asc.begin();
  auto di = dsc.begin();

  for (;;) {
    if (asc.end() == ai || dsc.end() == di) {
      std::cout << "NO";
      break;
    }

    const int cur = (*ai + *di);
    if (10000 == cur) {
      std::cout << "YES";
      break;
    }
    if (10000 > cur) {
      ++ai;
    } else {
      ++di;
    }
  }

  return 0;
}
