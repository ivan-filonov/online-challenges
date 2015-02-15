/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <sstream>
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
	process("another line");
	std::cout << "another" << std::endl;
	process("some line with text");
	std::cout << "some" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::string l;
	std::istringstream ss(s);
	std::string p;
	while(std::getline(ss, p, ' ')) {
		if(p.length() > l.length()) {
			std::swap(p, l);
		}
	}
	std::cout << l << std::endl;
}
