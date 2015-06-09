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
	return 0;
}

void process(std::string s);

void test() {
	process("40 40 40 40 29 29 29 29 29 29 29 29 57 57 92 92 92 92 92 86 86 86 86 86 86 86 86 86 86");
	std::cout << "4 40 8 29 2 57 5 92 10 86" << std::endl;
	process("73 73 73 73 41 41 41 41 41 41 41 41 41 41");
	std::cout << "4 73 10 41" << std::endl;
	process("1 1 3 3 3 2 2 2 2 14 14 14 11 11 11 2");
	std::cout << "2 1 3 3 4 2 3 14 3 11 1 2" << std::endl;
	process("7");
	std::cout << "1 7" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::istringstream ss(s);
	bool first = true;//item we just read is the first
	bool mid = false;//in the middle of output sequence
	int av = ~0;//accumulator value
	int ac = 0;//accumulator counter
	while(!ss.eof()) {
		int v;
		ss >> v;
		if(!first) {
			if(av == v) {
				++ac;
			} else {
				if(mid) {
					std::cout << ' ';
				} else {
					mid = true;
				}
				std::cout << ac << ' ' << av;
				ac = 1;
				av = v;
			}
		} else {
			first = false;
			av = v;
			ac = 1;
		}
	}
	std::cout << (mid?" ":"") << ac << ' ' << av << std::endl;

}
