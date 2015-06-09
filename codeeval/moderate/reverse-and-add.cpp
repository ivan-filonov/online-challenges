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
	process("195");
	std::cout << "4 9339" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

long rev(long v) {
	long r = 0;
	while(v) {
		r = r * 10 + v % 10;
		v /= 10;
	}
	return r;
}

void process(std::string s) {
	auto n = std::stol(s);
	int step = 0;
	decltype(n) r;
	while((r = rev(n)) != n) {
		++step;
		n += r;
	}
	std::cout << step << ' ' << n << std::endl;
}
