/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("");
	std::cout << ""  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string line) {
	std::cout << "s = '" << line << "'\n";
    std::istringstream ss { line };
}
