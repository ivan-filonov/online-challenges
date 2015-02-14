/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
	process_file(_v[1]);
}

void process(std::string s);

void test() {
	process("10");
	process("22");
	process("56");
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	auto n = std::stol(s);
	int res = 0;
	while(n) {
		res += (n&1);
		n >>= 1;
	}
	std::cout << res << std::endl;
}
