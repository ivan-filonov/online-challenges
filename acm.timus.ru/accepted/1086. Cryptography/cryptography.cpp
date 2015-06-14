#include <cmath>
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  vector<int> primes { 2, 3, 5, 7 };
  int l = 9;

  int k;
  std::cin >> k;

  while(k-->0) {
    int i;
    std::cin >> i;
    while(i > primes.size()) {
      bool flag = false;
      int sl = std::sqrt(l);
      for(int p : primes) {
        if(0 == l % p) {
          flag = true;
          break;
        }
        if(p > sl) {
          break;
        }
      }
      if(!flag) {
        primes.push_back(l);
      }
      l += 2;
    }
    std::cout << primes[i-1] << "\n";
  }

  return 0;
}
