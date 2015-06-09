/*
 * Common base C++ source for codeeval solutions.
 * */
#include <cctype>
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
	process("23");
	std::cout << "5" << std::endl;
	process("496");
	std::cout << "19" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	int sum = 0;
	for(char c : s) {
		if(std::isdigit(c)) {
			sum += c - '0';
		}
	}
	std::cout << sum << std::endl;
}
