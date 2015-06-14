#include <cmath>
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  
  int n, n0;
  double d;
  std::cin >> n >> d;
  d *= 3.1415926 * 2;
  n0 = n;
  
  double x0, y0, px, py, x, y;
  bool mid = false;
  while(n0-->0) {
    std::cin >> x >> y;
    if(mid) {
      d += std::sqrt((x - px) * (x - px) + (y - py) * (y - py));
    } else {
      x0 = x;
      y0 = y;
      mid = true;
    }

    px = x;
    py = y;
  }
  if(n > 1) {
    d += std::sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));
  }
  std::cout.setf(std::cout.fixed);
  std::cout.precision(2);
  std::cout << d << "\n";
  return 0;
}
