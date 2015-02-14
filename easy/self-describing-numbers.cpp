/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
	process_file(_v[1]);
}

void process(std::string s);

void test() {
	process("2020");
	std::cout << "1" << std::endl;
	process("22");
	std::cout << "0" << std::endl;
	process("1210");
	std::cout << "1" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	char t = '0';
	for(char c : s) {
		c -= '0';
		int cnt = 0;
		for(char cc : s) {
			if(cc == t) {
				++cnt;
			}
		}
		if(c != cnt) {
			t = -1;
			break;
		}
		++t;
	}
	std::cout << ((-1==t) ? '0' : '1') << std::endl;
}
