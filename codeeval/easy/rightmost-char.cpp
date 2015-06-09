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
	process("Hello World,r");
	std::cout << "8\n";// std::endl for flush?
	process("Hello World,x");
	std::cout << "-1\n";// std::endl for flush?
	process("Hello CodeEval,E");
	std::cout << "10\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	if(s.empty()) {
		return;
	}
	auto p = s.rfind(s.back(), s.length() - 2);
	if(std::string::npos == p) {
		std::cout << -1 << "\n";
	} else {
		std::cout << p << "\n";
	}
}
