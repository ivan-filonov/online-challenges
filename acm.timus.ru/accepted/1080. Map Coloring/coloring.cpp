#include <iostream>
#include <map>
#include <set>
#include <sstream>

using std::map;
using std::set;

static map<int, set<int>> graph;
static map<int, int> color;
static set<int> todo;

void paint(int from, int col) {
  color[from] = col;
  todo.erase(from);

  auto &links = graph[from];
  for (int i : links) {
    if (todo.count(i)) {
      paint(i, 1 - col);
    }
  }
}

int main() {
#ifdef ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"3\n2 0\n3 0\n0"};
#endif
  int N;
  cin >> N;

  for (int i = 1; i <= N; ++i) {
    for (int j; cin >> j && j;) {
      graph[i].insert(j);
      graph[j].insert(i);
    }
    color[i] = -1;
    todo.insert(i);
  }

  while (!todo.empty()) {
    const int pt = *todo.begin();
    todo.erase(todo.begin());
    int pc = color[pt];
    if (pc == -1) {
      pc = 0;
    }

    paint(pt, pc);
  }

  bool ok = true;
  for (auto &v : graph) {
    const int vc = color[v.first];
    for (auto &i : v.second) {
      if (vc == color[i]) {
        ok = false;
        break;
      }
    }
    if (!ok) {
      break;
    }
  }

  if (ok) {
    for (int i = 1; i <= N; ++i) {
      std::cout << color[i];
    }
  } else {
    std::cout << "-1";
  }
  std::cout << "\n";

  return 0;
}
