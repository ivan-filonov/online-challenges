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
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("6011 5940 0319 9511");
	std::cout << "0"    "\n";
	process("5537 0213 6797 6815");
	std::cout << "1"    "\n";
	process("5574 8363 8022 9735");
	std::cout << "0"    "\n";
	process("3044 8507 9391 30");
	std::cout << "0"    "\n";
	process("6370 1675 9034 6211 774");
	std::cout << "1"    "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

int c1 (int c) {
    return c - '0';
}

int c2(int c) {
    auto r = c1(c);
    if(r < 5) {
        return r * 2;
    } else {
        return 1 + (r - 5) * 2;
    }
}

void process(std::string line) {
    int sum = 0;
    for(int i = line.length() - 1, j = 1; i > -1; --i) {
        auto c = line[i];
        if(!std::isdigit(c)) {
            continue;
        }
        sum += j ? c1(c) : c2(c);
        j = !j;
    }
    std::cout << (0 == sum%10) << "\n";
}
