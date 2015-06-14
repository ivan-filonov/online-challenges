#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);
  int n;
  std::cin >> n;
  vector<int> v(n / 2 * 9 + 1);
  int j = 1;
  for(int i = 0; i < n/2; ++i) {
    j *= 10;
  }
  for(int i = 0; i < j; ++i) {
    int s = 0;
    for(int k = i; k; k /= 10) {
      s += k % 10;
    }
    ++v[s];
  }
  int s = 0;
  for(int i : v) {
    s += i * i;
  }
  std::cout << s << "\n";
  return 0;
}
