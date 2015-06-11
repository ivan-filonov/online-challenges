#include <iostream>
int main() {
  int k, n, w;
  std::cin >> k >> n >> w;
  if(w % 2) {
    k = ( k * (w + 1)) /2;
    k = k * w;
  } else {
    k = ( k * w ) / 2;
    k = k * (w + 1);
  }
  if(n >= k) {
    std::cout << 0;
  } else {
    std::cout << (k - n);
  }
  return 0;
}
