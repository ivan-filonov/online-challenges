/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
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
	process("22 2 2 9 11");
	std::cout << "3\n";// std::endl for flush?
	process("33 5 0");
	std::cout << "5\n";// std::endl for flush?
	process("16 3 2 6 10");
	std::cout << "0\n";// std::endl for flush?
	process("835 125 1 113");
	std::cout << "5\n";// std::endl for flush?
	process("47 5 0");
	std::cout << "8\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
//	std::cout << "TODO: procees line " << s << "\n";
	std::istringstream ss(s);
	int len, dist, n;
	ss >> len >> dist >> n;
	std::vector<int> vec;
	while(n--) {
		int t;
		ss >> t;
		vec.push_back(t);
	}
	using std::begin;
	using std::end;
	std::sort(begin(vec), end(vec));
	int cnt = 0;
	for(int o = 6; o < len - 5; ) {
		int b = 0;
		for(auto it = begin(vec); end(vec) != it && !b; ++it) {
			if(std::abs(o - *it) < dist) {
				b = *it;
			}
		}
		if(!b) {
			++cnt;
			o = o + dist;
		} else {
			o = b + dist;
		}
	}
	std::cout << cnt << "\n";
}
