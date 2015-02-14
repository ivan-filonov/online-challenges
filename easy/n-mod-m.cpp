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
	process("20,6");
	process("2,3");
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	auto delim = s.find(',');
	auto n = std::stoi(s);
	auto m = std::stoi(s.substr(delim+1));
	auto mm = m;
	while(n > (mm*2)) {
		mm *= 2;
	}
	if(n > mm) {
		n -= mm;
	}
	while(n > m) {
		n -= m;
	}
	std::cout << n << std::endl;
}
