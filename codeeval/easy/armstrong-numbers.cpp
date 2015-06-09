/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>
#include <valarray>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("6");
	std::cout << "True" << std::endl;
	process("153");
	std::cout << "True" << std::endl;
	process("351");
	std::cout << "False" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::valarray<int> digits(s.length());
	for(int i = 0; i < s.length(); ++i) {
		digits[i] = s[i] - '0';
	}
	auto acc = digits;
	for(int i = 1; i < s.length(); ++i) {
		acc *= digits;
	}
	std::cout << ((acc.sum() == std::stoi(s)) ? "True" : "False") << std::endl;
}
