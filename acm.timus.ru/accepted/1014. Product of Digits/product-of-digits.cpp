#include <algorithm>
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int n;
  std::cin >> n;
  vector<int> r;
  if(n < 10) {
    if(!n) n += 10;
    std::cout << n;
    return 0;
  }
  for(int p = 9; p > 1; --p) {
    while(n > 1 && 0 == n % p) {
      r.push_back(p);
      n /= p;
    }
  }
  if(n > 1) {
    std::cout << "-1\n";
    return 0;
  }
  std::sort(r.begin(), r.end());
  for(int i : r) {
    std::cout << i;
  }
  std::cout << "\n";
  return 0;
}
