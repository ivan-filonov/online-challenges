#include <cmath>
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  vector<long long> v;
  for(string s; std::cin >> s;) {
    v.emplace_back(std::stoll(s));
  }
  std::cout.setf(std::cout.fixed);
  std::cout.precision(4);
  for(auto it = v.rbegin(); it != v.rend(); ++it) {
    std::cout << std::sqrt((long double)*it) << "\n";
  }
  return 0;
}
