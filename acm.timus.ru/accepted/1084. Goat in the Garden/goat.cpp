#include <cmath>
#include <iostream>
#include <vector>

using std::string;
template <typename V> using vector = std::vector<V>;

int main() {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
  std::cin.sync_with_stdio(false);
  double side, cord;
  std::cin >> side >> cord;
  std::cout.precision(3);
  std::cout.setf(std::cout.fixed);

  side /= 2.0;
  if (side >= cord) {
    std::cout << (cord * cord * M_PI);
  } else if (side * std::sqrt(2.0) < cord) {
    std::cout << (side * side * 4.0);
  } else {
    double angle = std::acos(side / cord);
    double s1 = cord * side * std::sin(angle);
    double s2 = cord * cord * (M_PI / 4.0 - angle);
    std::cout << ((s1 + s2) * 4.0);
  }
  return 0;
}
