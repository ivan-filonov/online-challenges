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
	process("Hello,lloHe");
	std::cout << "True"  "\n";// std::endl for flush?
	process("Basefont,tBasefon");
	std::cout << "True"  "\n";// std::endl for flush?
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
    auto s1 = s.substr(delim + 1);
    s.erase(delim);
    std::cout << (std::string::npos == (s+s).find(s1) ? "False\n" :"True\n");
}
