/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
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
	process("2 0 6 3 1 6 3 1 6 3 1");
	std::cout << "6 3 1"  "\n";// std::endl for flush?
	process("3 4 8 0 11 9 7 2 5 6 10 1 49 49 49 49");
	std::cout << "49"  "\n";// std::endl for flush?
	process("1 2 3 1 2 3 1 2 3");
	std::cout << "1 2 3"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
    std::istringstream ss { s };
    std::vector<int> v;
    for(std::string t; std::getline(ss, t, ' ');) {
        v.push_back(std::stoi(t));
    }
    const auto b = v.begin();
    for(int i = 0; i < v.size() - 1; ++i) {
        for(int j = 1; j < (v.size() - i)/2; ++j) {
            if(std::equal(b + i, b + i + j, b + i + j)) {
                bool mid = false;
                for(auto it = b + i; b + i + j != it; ++it) {
                    if(mid) {
                        std::cout << ' ';
                    } else {
                        mid = true;
                    }
                    std::cout << *it;
                }
                std::cout << "\n";
                return;
            }
        }
    }
}
