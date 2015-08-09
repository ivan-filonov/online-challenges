#include <iostream>
#include <vector>
#include <set>

using std::string;
template<typename V> using vector = std::vector<V>;
template<typename V> using set = std::set<V>;

struct ent {
	int id;
	set<int> anc;
	bool mark;

	ent() : id(1), mark(false) {}
};

int main() {
	std::cin.sync_with_stdio(false);

	int n;
	std::cin >> n;

	vector<ent> v(n);
	for(int i = 0; i != n; ++i) {
		v[i].id = i + 1;
		for(int s; std::cin >> s && s;) {
			v[s-1].anc.insert(i + 1);
		}
	}

	for(bool flag = true, mid = false; flag; mid = true) {
		flag = false;
		for(auto & e : v) {
			if(!e.mark && e.anc.empty()) {
				std::cout << (mid ? " " : "") << e.id;
				e.mark = true;
				for(auto & t : v) {
					t.anc.erase(e.id);
				}
				flag = true;
				break;
			}
		}
	}
	
	return 0;
}
