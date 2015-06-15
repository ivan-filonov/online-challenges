#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);

  int n;
  std::cin >> n;

  vector<int> v(n * n);
  for(int s = 0, i = 1; s < n * 2; ++s) {
    int x = n - 1 - s;
    int y = 0;
    if(x < 0) {
      y -= x;
      x = 0;
    }

    while(y < n && x < n) {
      v[y * n + x] = i;
      ++i;
      ++y;
      ++x;
    }
  }

  for(int line = 0, idx = 0; line < n; ++line) {
    for(int col = 0; col < n; ++col, ++idx) {
      std::cout << (col ? " " : "") << v[idx];
    }
    std::cout << "\n";
  }

  return 0;
}
