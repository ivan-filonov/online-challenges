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
	process("Hello world");
	std::cout << "Hello World" << std::endl;
	process("javaScript language");
	std::cout << "JavaScript Language" << std::endl;
	process("a letter");
	std::cout << "A Letter" << std::endl;
	process("1st thing");
	std::cout << "1st Thing" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	s[0] = std::toupper(s[0]);
	for(int i = 1; i < s.length(); ++i) {
		if(' ' == s[i-1]) {
			s[i] = std::toupper(s[i]);
		}
	}
	std::cout << s << std::endl;
}
