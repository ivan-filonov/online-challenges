#include <algorithm>
#include <iostream>
#include <vector>
using num_t = long long int;
static std::vector<num_t> primes{2, 3, 5};
static num_t incr = 4;
static num_t cur = 7;
num_t prime(size_t n) {
  while (n >= primes.size()) {
    num_t j = -1;
    for (j = -1; j < 0; cur += incr, incr = 6 - incr) {
      j = cur;
      for (auto &p : primes) {
        if (cur % p == 0) {
          j = -1;
          break;
        }
      }
      if (j > 0)
        break;
    }
    primes.push_back(cur);
  }
  return primes[n];
}
int main() {
  std::cin.sync_with_stdio(false);

  int numTests;
  std::cin >> numTests;

  while (numTests-- > 0) {
    num_t A, B, C;
    std::cin >> A >> B >> C;

    const num_t S = A * B;
    const num_t CC = C * C;

    if (S <= CC + CC / 2) {
      std::cout << CC << "\n";
      continue;
    }

    num_t r = S % CC;
    num_t r1 = CC - r;

    if (r < r1) {
      std::cout << (S - r) << "\n";
    } else {
      std::cout << (S + r1) << "\n";
    }
  }

  return 0;
}
