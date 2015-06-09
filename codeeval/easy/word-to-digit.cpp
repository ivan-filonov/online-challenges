/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

std::map<std::string,int> map;

void init_map();
void test();
void process_file(char*);
void process(std::string s);
int main(int _a, char ** _v) {
	init_map();
//	test();
	process_file(_v[1]);
	return 0;
}

void init_map() {
	auto words = { 
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
	};
	int i = 0;
	for(auto it = std::begin(words); std::end(words) != it; ++it, ++i) {
		map[*it] = i;
	}
}

void test() {
	process("zero;two;five;seven;eight;four");
	std::cout << "025784" << std::endl;
	process("three;seven;eight;nine;two");
	std::cout << "37892" << std::endl;
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
	std::string k;
	while(std::getline(ss, k, ';')) {
		std::cout << map[k];
	}
	std::cout << std::endl;
}
