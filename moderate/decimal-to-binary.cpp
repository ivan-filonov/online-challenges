/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("2");
	std::cout << "10\n";// std::endl for flush?
	process("10");
	std::cout << "1010\n";// std::endl for flush?
	process("67");
	std::cout << "1000011\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	auto i = std::stol(s);
	if(!i) {
		std::cout << "0\n";
		return;
	}
	std::string repr;
	repr.reserve(128);
	while(i) {
		repr += '0' + (i&1);
		i >>= 1;
	}
	std::reverse(std::begin(repr), std::end(repr));
	std::cout << repr << "\n";
}
