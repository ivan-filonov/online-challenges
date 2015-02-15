/*
 * Common base C++ source for codeeval solutions.
 * */
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
	process("0");
	std::cout << "0" << std::endl;
	process("5");
	std::cout << "2" << std::endl;
	process("101");
	std::cout << "1" << std::endl;
	process("25684");
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
//	std::cout << "TODO: procees line " << s << std::endl;
	auto n = std::stol(s);
	int bits = 0;
	while( n > 0 ) {
		bits += n & 1;
		n >>= 1;
	}
	std::cout << bits%3 << std::endl;
}
