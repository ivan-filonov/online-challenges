#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

using std::string;
template <typename V> using vector = std::vector<V>;

int main() {
#ifdef ONLINE_JUDGE
  using std::cin;
  std::cin.sync_with_stdio(false);
#else
  std::istringstream cin{"4 9"};
#endif
  int len;
  int num;
  cin >> len >> num;

  int fib[45] = {
      1, 1,
  };
  for (int i = 2; i < 45; ++i) {
    fib[i] = fib[i - 1] + fib[i - 2];
  }
  if (num > (fib[len] + fib[len - 1])) {
    std::cout << "-1\n";
  } else {

    for (int i = len, prev = 0; i > 0; --i) {
      const int c0 = fib[i];
      const int c1 = fib[i ? i : 0];
      if (num > c0 && !prev) {
        prev = 1;
        num -= c0;
      } else {
        prev = 0;
        if (num > c1)
          num -= c1;
      }
      std::cout << prev;
    }
    std::cout << "\n";
  }

  return 0;
}
