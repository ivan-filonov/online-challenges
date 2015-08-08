#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
	std::cin.sync_with_stdio(false);

	vector<vector<int>> v(101);

	int N;
	std::cin >> N;
	for(int i = 0; i != N; ++i) {
		int id;
		int m;
		std::cin >> id >> m;
		v[m].push_back(id);
	}

	for(int i = 100; i > -1; --i) {
		if(v[i].empty()) {
			continue;
		}
		for(auto j : v[i]) {
			std::cout << j << ' ' << i << "\n";
		}
	}

	return 0;
}
