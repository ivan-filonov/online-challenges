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
//	test();
	process_file(_v[1]);
}

void process(std::string s);

void test() {
	process("86,2,3");
	std::cout << "true" << std::endl;
	process("125,1,2");
	std::cout << "false" << std::endl;
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
	std::istringstream ss(s);
	int a,b,c;
	char d;
	ss >> a >> d >> b >> d >> c;
//	std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
	b = a >> (b-1);
	c = a >> (c-1);
	std::cout << ((c&1)==(b&1) ? "true" : "false") << std::endl;
}
