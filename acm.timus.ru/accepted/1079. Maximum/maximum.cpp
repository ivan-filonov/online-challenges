#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  vector<int> a { 0, 1 };
  for(int n; std::cin >> n && n;) {
    if(a.size() <= n) {
      int old_size = a.size();
      a.resize(n + 1);
      for(int i = old_size; i <= n; ++i) {
        if(i % 2) {
          a[i] = a[i/2] + a[i/2 + 1];
        } else {
          a[i] = a[i/2];
        }
      }
    }

    int res = 0;
    for(int i = 1; i <= n; ++i) {
      if(a[i] > res) {
        res = a[i];
      }
    }
    std::cout << res << "\n";
  }
  return 0;
}
