#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
	std::cin.sync_with_stdio(false);
	
	int n;
	std::cin >> n;
	
	if(1 == n) {
		std::cin >> n;
		std::cout << n;
	} else {
		int a, b;
		std::cin >> b;
		for(int i = 1; i != n; ++i) {
			std::cin >> a;
			if(a == b) {
				continue;
			}
			while(a && b) {
				if(a > b) {
					a %= b;
				} else {
					b %= a;
				}
			}
			b += a;
		}
		std::cout << b << "\n";
	}

	return 0;
}
