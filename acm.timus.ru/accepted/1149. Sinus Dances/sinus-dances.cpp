#include <iostream>
#include <vector>
#include <sstream>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);

  int n;
  std::cin >> n;

  std::ostringstream pA, sA;
  
  for(int i = 1; i < n; ++i) {
    std::cout << "(";
  }

  for(int i = 1, j = n; i <= n; ++i, --j) {
    if(i > 1) {
      pA << (i % 2 ? "+" : "-"); 
    }
    pA << "sin(" << i;
    sA << ")";

    std::cout << pA.str() << sA.str() << "+" << j;

    if(j > 1) {
      std::cout << ")";
    }
  }

  return 0;
}
