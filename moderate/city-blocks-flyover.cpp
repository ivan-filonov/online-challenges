/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <cmath>
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
	process("(0,2,4,8,10,13,14,18,22,23,24,33,40,42,44,47,49,53,55,63,66,81,87,91) (0,147,220)");
	std::cout << "24" << std::endl;
	process("(0,1,2,4) (0,1,3,4,5)");
	std::cout << "6" << std::endl;
	process("(0,1,3,4,6) (0,1,2,4)");
	std::cout << "5" << std::endl;
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
	ss.ignore(10, '(');
	double v;
	char c;
	std::vector<double> horz, vert;
	do {
		ss >> v >> c;
		vert.push_back(v);
	} while(',' == c);
	ss.ignore(10, '(');
	do {
		ss >> v >> c;
		horz.push_back(v);
	} while(',' == c);
	// std::cout << "vert.size() = " << vert.size() << ", horz.size() = " << horz.size() << std::endl;
	// line coeffs a*x+b*y=0
	const double a = horz.back(), b = vert.back();
	auto	cur_h = std::begin(horz), end_h = std::end(horz),
		cur_v = std::begin(vert), end_v = std::end(vert);
	int n = 0;
	if(0 == *cur_h || 0 == *cur_v) {
		if(0 == *cur_h) { ++cur_h; }
		if(0 == *cur_v) { ++cur_v; }
	}
	while(cur_h != end_h || cur_v != end_v) {
		// cell top: y = cur_h, intersection is x = -(b/a) * cur_h, y = cur_h
		// cell right: x = cur_v, intersection is x = cur_v, y = -(a/b) * cur_v
		auto ix_top = (*cur_h) * b / a;
		auto iy_right = (*cur_v) * a / b;
		// std::cout << "a = " << a << ", b = " << b << ", *cur_h = " << *cur_h << ", *cur_v = " << *cur_v << ", ix_top = " << ix_top << ", iy_right = " << iy_right << std::endl;

		// test one: exiting cell through its corner
		if(std::abs(ix_top - *cur_v) < 1e-9 && std::abs(iy_right - *cur_h) < 1e-9 ) {
			++cur_h;
			++cur_v;
			++n;	
		}
		// test two: exiting cell through top border
		else if (ix_top < *cur_v) {
			++cur_h;
			++n;
		} else {
		// exiting cell through right border
		 	++cur_v;
		 	++n;
		}
	}
	std::cout /*<< "n = "*/ << n << std::endl;
}
