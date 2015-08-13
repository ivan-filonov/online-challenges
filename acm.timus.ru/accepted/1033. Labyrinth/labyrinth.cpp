#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
	std::cin.sync_with_stdio(false);
	
	int n;
	std::cin >> n;

	vector<char> m((n + 2) * (n + 2), '#');

	for(int i = 0; i != n; ++i) {
		string t;
		std::cin >> t;
		for(int j  = 0; j != n; ++j) {
			m[(n + 2) * i + n + 3 + j] = t[j];
		}
	}

	m[0] = m[1] = m[n + 2] = '+';
	m[n*n + 4*n + 3] = m[n*n + 4*n + 2] = m[n*n + 3*n + 1] = '+';

	const int d[4] = { 1, n + 2, -1, - n - 2 };

	vector<int> o { n + 3, n*n + 3*n };
	int res = 0;
	while(!o.empty()) {
		int cur = o.back();
		o.pop_back();

		if('.' != m[cur]) {
			continue;
		}

		m[cur] = '+';

		for(int i : d) {
			switch(m[cur + i]) {
				case '.':
					o.push_back(cur + i);
					break;
				case '#':
					++res;
					break;
			}
		}
	}

	if(0)
	for(int l = 0; l != n + 2; ++l) {
		std::cout << string(&m[(n+2)*l], &m[(n+2)*(l+1)]) << "\n";
	}

	std::cout << (res * 9);

	return 0;
}
