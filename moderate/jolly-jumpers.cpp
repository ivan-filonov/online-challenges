/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
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
	process("4 1 4 2 3");
	std::cout << "Jolly" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::istringstream ss(s);
	int n;
	ss >> n;
	std::vector<int> nums;
	typedef std::istream_iterator<int> _iter_t;
	std::copy(_iter_t(ss), _iter_t(), std::back_inserter(nums));
	for(auto it = std::begin(nums) + 1; std::end(nums) != it; ++it) {
		*(it-1) = std::abs(*(it-1) - *it);
	}
	nums.erase(std::end(nums) - 1);
	std::set<int> set;
	set.insert(std::begin(nums), std::end(nums));
	auto p = std::minmax_element(std::begin(nums), std::end(nums));
	bool t = set.size() == nums.size();
	t &= 1==*p.first;
	t &= (n-1) == *p.second;
	std::cout << (t ? "J" : "Not j") << "olly" << std::endl;
}
