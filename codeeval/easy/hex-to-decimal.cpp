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
}

void process(std::string s);

void test() {
	process("9f");
	std::cout << "159" << std::endl;
	process("11");
	std::cout << "17" << std::endl;
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
	long v = 0;
	for(char c : s) {
		v *= 16;
		if('0' <= c && c <= '9') {
			v += c - '0';
		} else if('a' <= c && c <= 'f') {
			v += c - 'a' + 10;
		}
	}
	std::cout << v << std::endl;
}
