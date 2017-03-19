#include <iostream>
#include <vector>
int main() {
  int n;
  std::cin >> n;
  std::vector<int64_t> f(n);
	int64_t sum = 0;
  for (auto &i : f) {
    std::cin >> i;
		sum += i;
  }
	int j = -1;
	for(ssize_t i = 0; i < n; ++i) {
		if(-1 == j && f[i] * 2 == sum) {
			j = i;
			continue;
		}
		std::cout << f[i] << ' ';
	}
	std::cout << f[j] << "\n";
}
