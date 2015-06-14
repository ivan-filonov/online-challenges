#include <algorithm>
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int n;
  std::cin >> n;
  vector<int> weights;
  weights.resize(n);
  for(int i = 0; i != n; ++i) {
    std::cin >> weights[i];
  }
  
  int sum = std::accumulate(weights.begin(), weights.end(), 0), d = sum;
  for(int i = 1; i != 1 << n; ++i) {
    int csum = sum;
    for(int j = 1, k = 0; k != n; j <<= 1, ++k) {
      if(i & j) {
        csum -= weights[k] * 2;
        if(csum < 0) {
          break;
        }
      }
    }
    if(csum >= 0) {
      if(d > csum) {
        d = csum;
      }
      if(!d) {
        break;
      }
    }
  }

  std::cout << d << "\n";

  return 0;
}
