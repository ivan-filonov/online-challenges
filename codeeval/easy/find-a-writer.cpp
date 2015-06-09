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
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("osSE5Gu0Vi8WRq93UvkYZCjaOKeNJfTyH6tzDQbxFm4M1ndXIPh27wBA rLclpg| 3 35 27 62 51 27 46 57 26 10 46 63 57 45 15 43 53");
	std::cout << "Stephen King 1947" << std::endl;
	process("3Kucdq9bfCEgZGF2nwx8UpzQJyHiOm0hoaYP6ST1WM7Nks5XjrR4IltBeDLV vA| 2 26 33 55 34 50 33 61 44 28 46 32 28 30 3 50 34 61 40 7 1 31");
	std::cout << "Kyotaro Nishimura 1930" << std::endl;
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
	ss.ignore(s.length(), '|');
	while(!ss.eof()) {
		int i;
		ss >> i;
		std::cout << s[i-1];
	}
	std::cout << std::endl;
}
