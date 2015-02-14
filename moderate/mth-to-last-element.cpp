/*
 * Common base C++ source for codeeval solutions.
 * */
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
}

void process(std::string s);

void test() {
	process("a b c d 4");
	std::cout << "a" << std::endl;
	process("e f g h 2");
	std::cout << "g" << std::endl;
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
	std::vector<std::string> parts;
	while(!ss.eof()) {
		ss >> s;
		parts.push_back(s);
	}
	auto n = std::stoi(parts.back());
//	std::cout << "n = " << n << ", parts.size() = " << parts.size() << std::endl;
	n = parts.size() - 1 - n;
//	std::cout << "n = " << n << std::endl;
	if(n >= 0) {
		std::cout << parts[n] << std::endl;
	}
}
