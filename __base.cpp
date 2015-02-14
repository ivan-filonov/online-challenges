/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("");
	std::cout << "" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::cout << "TODO: procees line " << s << std::endl;
}
