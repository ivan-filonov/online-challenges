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
	process("how are you, abc");
	std::cout << "how re you" << std::endl;
	process("hello world, def");
	std::cout << "hllo worl" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	auto comma = s.rfind(',');
	auto chars = s.substr(comma + 2);
	s = s.substr(0, comma);
	size_t i;
	while(std::string::npos != (i = s.find_first_of(chars))) {
		s.erase(i,1);
	}
	auto lns = s.find_last_not_of(' ');
	s = s.substr(0, lns + 1);
	std::cout << s << std::endl;
}
