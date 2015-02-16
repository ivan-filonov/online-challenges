/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <stdio.h>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("70.920 -38.797 14.354 99.323 90.374 7.581");
	std::cout << "-38.797 7.581 14.354 70.920 90.374 99.323" << std::endl;
	process("-37.507 -3.263 40.079 27.999 65.213 -55.552");
	std::cout << "-55.552 -37.507 -3.263 27.999 40.079 65.213" << std::endl;
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
	std::vector<double> vec;
	typedef std::istream_iterator<double> _iter_t;
	std::copy(_iter_t(ss), _iter_t(), std::back_inserter(vec));
	std::sort(std::begin(vec), std::end(vec));
	for(auto b = std::begin(vec), e = std::end(vec), it = b; e != it; ++it) {
		if( b != it ) {
			printf(" ");
		}
		printf("%.3f", *it);
	}
	printf("\n");
}
