#include <deque>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

int main() {
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"7 2 3 0 3 1 0 1 2 4 5 0 3 0 3 0 7 0 6 0"};
#endif

  int num_ppl;
  cin >> num_ppl;
  std::vector<std::vector<int>> g(1);
  for (int i = 1; i <= num_ppl; ++i) {
    g.emplace_back();
    auto &gg = g[i];
    for (int j; cin >> j && j;) {
      gg.push_back(j);
    }
  }

  for (int i = 1; i <= num_ppl; ++i) {
    if (g[i].empty()) {
      std::cout << "0\n";
      return 0;
    }
  }

  std::vector<int> team(num_ppl + 1, 0);
  std::set<int> todo;
  for (int i = 1; i <= num_ppl; ++i) {
    todo.insert(i);
  }

  std::deque<int> queue;
  while (!todo.empty()) {
    const int seed = *todo.begin();
    todo.erase(todo.begin());
    team[seed] = 1;
    queue.push_back(seed);
    while (!queue.empty()) {
      const int cur = queue.front();
      queue.pop_front();
      for (auto i : g[cur]) {
        if (team[i]) {
          continue;
        }
        team[i] = 3 - team[cur];
        todo.erase(i);
        queue.push_back(i);
      }
    }
  }

  int r = 0;
  for (int i : team) {
    if (i == 1) {
      ++r;
    }
  }

  std::cout << r << "\n";
  int f = 0;
  for (int i = 1; i <= num_ppl; ++i) {
    if (team[i] == 1) {
      std::cout << (f++ ? " " : "") << i;
    }
  }
  std::cout << "\n";

  return 0;
}
