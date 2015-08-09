#include <iostream>
int main() {
	int t;
	std::cin >> t;
	for(int test = 0; test != t; ++test) {
		int n, k;
		std::cin >> n >> k;
		for(int i = 0; i != n; ++i) {
			int a;
			std::cin >> a;
			if(a <= 0) {
				--k;
			}
		}
		std::cout << (k <= 0 ? "NO" : "YES") << "\n";
	}
	return 0;
}
