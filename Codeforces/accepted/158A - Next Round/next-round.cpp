#include <iostream>
int main() {
	int n;// total
	int k;// k-th place
	std::cin >> n >> k;

	int cut;
	int res = 0;
	for(int i = 0; i != n; ++i) {
		int a;
		std::cin >> a;
		if(a) {
			if(i + 1 <= k) {
				cut = a;
				++res;
			} else if(a == cut) {
				++res;
			}
		}
	}
	std::cout << res;

	return 0;
}
