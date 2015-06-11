#include <iostream>

int main() {
  long long m, n, a, c;
  std::cin >> m >> n >> a;
  c = m % a;
  m = m / a + (c ? 1 : 0);
  c = n % a;
  n = n / a + (c ? 1 : 0);
  std::cout << (m * n);
  return 0;
}

