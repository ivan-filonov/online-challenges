/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("3 3 9 1 6 5 8 1 5 3");
	std::cout << "5" << std::endl;
	process("9 2 9 9 1 8 8 8 2 1 1");
	std::cout << "0" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::vector<int> nv;
	std::istringstream ss(s);
	typedef std::istream_iterator<int> _iter_t;
	std::copy(_iter_t(ss), _iter_t(), std::back_inserter(nv));
	std::map<int,int> map;
	std::for_each(std::begin(nv), std::end(nv), [&map](int i) { ++map[i]; } );
	for(auto it = std::begin(map), end = std::end(map); end != it;) {
		if(it->second > 1) {
			it = map.erase(it);
		} else {
			++it;
		}
	}
	int res = 0;
	if(!map.empty()) {
		auto p = std::find(std::begin(nv), std::end(nv), std::begin(map)->first);
		res = p - std::begin(nv) + 1;
	}
	std::cout << res << std::endl;
}
