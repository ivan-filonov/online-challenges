#include <iostream>
#include <map>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;
using coords = std::pair<int, int>;
template<typename K, typename V> using map = std::map<K,V>;

int main() {
	std::cin.sync_with_stdio(false);
	int n;
	std::cin >> n;

	vector<coords> c(n);

	for(int i = 0; i !=n; ++i) {
		std::cin >> c[i].first >> c[i].second; 
	}

	vector<int> primes { 2, 3 };
	while(primes.back() < 5000) {
		for(int p = primes.back() + 1;; ++p) {
			bool prime = true;
			for(int k : primes) {
				if(!(p%k)) {
					prime = false;
					break;
				}
			}
			if(prime) {
				primes.push_back(p);
				break;
			}
		}
	}

	map<int,map<int,map<int,vector<int>>>> m;
	size_t ml = 0;
	for(int from = 0; from != n; ++from) {
		for(int to = 0; to !=n; ++to) {
			if(from == to) {
				continue;
			}
			auto dx = c[to].first - c[from].first;
			auto dy = c[to].second - c[from].second;
			if(dx <= 0 && dy <= 0) {
				dx = -dx;
				dy = -dy;
			}
			if(!dx) {
				dy = 1;
			} else if(!dy) {
				dx = 1;
			} else {
				if(dx < 0) {
					dx = -dx;
					dy = -dy;
				}
				int m = std::min(std::abs(dx), std::abs(dy));
				for(int p : primes) {
					if(p > m) {
						break;
					}
					while(!(dx%p) && !(dy%p)) {
						dx /= p;
						dy /= p;
					}
				}
			}
			auto & v = m[dx][dy][from];
			v.push_back(to);
			if(v.size() > ml) {
				ml = v.size();
			}
		}
	}

	++ml;
	std::cout << ml;

	return 0;
}
