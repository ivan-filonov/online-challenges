#include <iostream>
#include <sstream>

int main() {
  using nt = long long int;
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"2"};
#endif
  nt n;
  cin >> n;
	std::cout << n*(n+1)*(n+2)/2 << "\n";
  return 0;
}
