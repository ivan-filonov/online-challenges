/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <array>
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
	process("3 1");
	std::cout << "2"    "\n";
	process("100 100");
	std::cout << "50"    "\n";
    // std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

void process(std::string line) {
    size_t d;
    const auto N = std::stoi(line, &d);
    const auto M = std::stoi(line.substr(d+1));
    std::vector<bool> v(N,false);
    for(int i = 1; i < M; ++i) {
        for(int j = 1; j < N; j += 2) {
            v[j] = true;
        }
        for(int j = 2; j < N; j += 3) {
            v[j] = !v[j];
        }
    }
    v.back() = !v.back();
    std::cout << std::count(std::begin(v), std::end(v), false) << "\n";
}
