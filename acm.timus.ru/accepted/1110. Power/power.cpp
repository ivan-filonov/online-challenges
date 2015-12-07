#include <iostream>
#include <vector>

using std::string;
template <typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int N, M, Y, mid = 0;
  std::cin >> N >> M >> Y;
  for (int x = 0; x < M; ++x) {
    int y = 1;
    for (int z = 0; z < N; ++z)
      y = (y * x) % M;
    if (y == Y) {
      std::cout << (mid ? " " : "") << x;
			mid = 1;
    }
  }
  if (!mid)
    std::cout << -1;
  return 0;
}
