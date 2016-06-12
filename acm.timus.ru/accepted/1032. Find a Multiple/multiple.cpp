#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

template <typename K, typename V> using umap = std::unordered_map<K, V>;
template <typename V> using vector = std::vector<V>;

static int N;
vector<int> edges;
vector<bool> visited;

int *dfs(int current, int *begin, int *end) {
  current %= N;
  if (!current) {
    return end;
  }

  visited[current] = true;
  for (auto ptr = begin; ptr != end; ++ptr) {
    int next = (current + *ptr) % N;
    if (visited[next]) {
      continue;
    }

    auto nend = end - 1;
    std::swap(*ptr, *nend);
    auto res = dfs(next, begin, nend);
    if (res) {
      return res;
    }
    std::swap(*ptr, *nend);
  }

  return nullptr;
}

int main() {
#ifdef ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"5 1 2 3 4 1"};
#endif
  cin >> N;

  edges.resize(N);
  visited.assign(N, false);
  for (int &i : edges) {
    cin >> i;
  }

  int *const begin = &edges[0];
  int *nend = std::addressof(edges.back());
  int *end = nend + 1;
  int *solution = nullptr;
  for (auto ptr = begin; ptr != end; ptr++) {
    const int current = *ptr;
    std::swap(*ptr, *nend);
    solution = dfs(current, begin, nend);
    if (solution) {
      break;
    }
    std::swap(*ptr, *nend);
  }

  if (solution) {
    std::cout << std::distance(solution, end) << "\n";
    while (solution != end) {
      std::cout << *solution << "\n";
      ++solution;
    }
  } else {
    std::cout << "0\n";
  }

  return 0;
}
