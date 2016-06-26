#include <iostream>
#include <sstream>

int main() {
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"3 5"};
#endif
  using num_t = long long int;
  num_t rows, cols;
  cin >> rows >> cols;

  auto m = std::min(rows , cols )-1;
  num_t result = m * 2;

  if (rows > cols) {
    ++result;
  }

  std::cout << result << "\n";

  return 0;
}
