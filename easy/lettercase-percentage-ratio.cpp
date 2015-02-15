/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("thisTHIS");
	std::cout << "lowercase: 50.00 uppercase: 50.00" << std::endl;
	process("AAbbCCDDEE");
	std::cout << "lowercase: 20.00 uppercase: 80.00" << std::endl;
	process("N");
	std::cout << "lowercase: 0.00 uppercase: 100.00" << std::endl;
	process("UkJ");
	std::cout << "lowercase: 33.33 uppercase: 66.67" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
//	std::cout << "TODO: procees line " << s << std::endl;
	double l  { 0 };
	double u  { 0 };
	for(char c : s) {
		if(std::isupper(c)) {
			u += 1;
		} else if (std::islower(c)) {
			l += 1;
		}
	}
	printf("lowercase: %.2f uppercase: %.2f\n", 100.0*l/(l+u), 100.0*u/(l+u));
}
