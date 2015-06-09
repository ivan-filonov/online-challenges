/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
	for(int i = 1; i <= 12; ++i) {
		std::cout << std::setw(0);
		for(int j = 1; j <= 12; ++j) {
//			if(i > 9) {
//				if(2 == j) {
//					std::cout << std::setw(3);
//				}
//				else if (3 <= j) {
//					std::cout << std::setw(4);
//				}
//			} else {
				if(j > 1) {
					std::cout << std::setw(4);
				}
//			}
			std::cout << i * j;
		}
		std::cout << std::endl;
	}
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
