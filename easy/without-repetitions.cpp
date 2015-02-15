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
	return 0;
}

void process(std::string s);

void test() {
	process("Shellless mollusk lives in wallless house in wellness. Aaaarrghh!");
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	char prev = 0;
	for(auto c : s) {
		if(/*!std::isalpha(prev) ||*/ prev != c) {
			std::cout << c;
		}
		prev = c;
	}
	std::cout << std::endl;
}
