#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  
  int n;
  std::cin >> n;

  vector<int> v(n*n);
  for(int & i : v) {
    std::cin >> i;
  }

  for(int starty = 0; starty < 2 * n; ++starty) {
    int y = starty;
    for(int x = 0; y >= 0;){
      if(x < n && y < n) {
        std::cout << (starty ? " " : "") << v[n * y + x];
      }
      ++x;
      --y;
    }
  }

  return 0;
}
