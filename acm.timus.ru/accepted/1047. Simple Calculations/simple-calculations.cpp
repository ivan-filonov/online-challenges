#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
	std::cin.sync_with_stdio(false);

	int n;
	std::cin >> n;

	double a0, an1;
	std::cin >> a0 >> an1;

	// a[i] = a[i-1]/2 + a[i+1]/2 - c[i]
	// 2*a[i] = a[i-1] + a[i+1] - 2*c[i]
	// a[i] - a[i-1] = a[i+1] - a[i] - 2*c[i]
	// a[i+1] - a[i] = a[i] - a[i-1] + 2*c[i]
	// d[i+1] = d[i] + 2*c[i]
	// d[i+k] = d[i] + 2*(c[i] + ... + c[i+k-1])
	// a[1] = a[0] + d[1]
	// a[k] = a[0] + d[1]*k + 2*(c[1]*(k-1) + ... + c[k-1])
	// a[n+1] = a[0] + d[1]*(n+1) + 2*(c[1]*n + ... + c[n])
	
	double s = 0;
	for(int i = 0, j = n; i != n; ++i) {
		double c;
		std::cin >> c;
		s += c * j--;
	}

	std::cout.precision(2);
	std::cout.setf(std::cout.fixed);
	std::cout << (a0 + (an1 - a0 - 2 * s) / (n + 1));

	return 0;
}
