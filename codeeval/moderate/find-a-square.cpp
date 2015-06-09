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
	process("(1,6), (6,7), (2,7), (9,1)");
	std::cout << "false"  "\n";// std::endl for flush?
	process("(4,1), (3,4), (0,5), (1,2)");
	std::cout << "false"  "\n";// std::endl for flush?
	process("(4,6), (5,5), (5,6), (4,5)");
	std::cout << "true"  "\n";// std::endl for flush?
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
	std::vector<int> x(4), y(4), p(3);
	for(int i = 0; i < 4; ++i) {
		std::string t;
		ss.ignore(10, '(');
		std::getline(ss, t, ',');
		x[i] = std::stoi(t);
		std::getline(ss, t, ')');
		y[i] = std::stoi(t);
	}
	for(int i = 0; i < 3; ++i) {
		x[i] -= x[3];
		y[i] -= y[3];
	}
	int m = -1;
	for(int i = 0; i < 3; ++i) {
		const auto j = (i + 1) % 3;
		if(!(p[i] = p[i] = x[i]*x[j] + y[i]*y[j])) {
			m = i;
		}
	}
	if(m > -1) {
		if( 1 != std::count(std::begin(p), std::end(p), 0) ) {
			m = -1;
		}
	}
	if(m > -1) {
		const auto n = (m + 1) % 3;
		const auto k = (n + 1) % 3;
		if((x[m]+x[n] != x[k]) || (y[m]+y[n] != y[k])) {
			m = -1;
		}
	}
	std::cout << (m > -1 ? "true\n" : "false\n");
}
