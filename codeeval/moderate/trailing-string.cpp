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
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
    process("Hello World,World");
	std::cout << "1"    "\n";
    process("Hello CodeEval,CodeEval");
	std::cout << "1"    "\n";
    process("San Francisco,San Jose");
	std::cout << "0"    "\n";
    // std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

void process(std::string line) {
//    std::istringstream ss { line };
    int delim = line.find(',');
    int lend = line.length() - 1;
    int elen = lend - delim;
    int ipos = delim - elen;
    auto b = std::begin(line);
    std::cout << (ipos >= 0 && std::equal(b + delim + 1, b + lend, b + ipos)) << "\n";
}
