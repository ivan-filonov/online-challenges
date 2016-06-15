#include <iostream>
#include <sstream>
int main() {
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"4"};
#endif
  unsigned long long int N;
  cin >> N;

  unsigned long long int fib[46] = {
      1, 1,
  };
  for (int i = 2; i < 46; ++i) {
    fib[i] = fib[i - 1] + fib[i - 2];
  }
  std::cout << (fib[N - 1] * 2) << "\n";

  return 0;
}
