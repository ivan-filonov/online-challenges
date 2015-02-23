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
    if( !1 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

void process(std::string s);

void test() {
	process("11");
	std::cout << "3"    "\n";
	process("20");
	std::cout << "4"    "\n";
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
    auto v = std::stoi(line);
    int n = v / 5;
    v %= 5;
    if( v >= 3 ) {
        n += 1;
        v -= 3;
    }
    n += v;
    std::cout << n << "\n";
}
