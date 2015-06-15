#include <iostream>
#include <vector>
#include <cmath>

using std::string;
template<typename V> using vector = std::vector<V>;

int bit(long long k) {
  if(1 == k) {
    return 1;
  } 
  --k;
  // need j | j * (j + 1) / 2 == k
  long long j = std::sqrt(2ll * k);
  long long l = j * (j + 1) / 2;
  return k == l ? 1 : 0;
}

int main() {
  std::cin.sync_with_stdio(false);

  int n;
  std::cin >> n;

  for(int k; n-->0 && std::cin >> k; 0) {
    std::cout << bit(k) << (n ? " " : "");
  }

  return 0;
}
