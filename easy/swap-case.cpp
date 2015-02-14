/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
}

void process(string s);

void test() {
	process("Hello world!");
	process("JavaScript language 1.8");
	process("A letter");
}

void process_file(char* path) {
	std::ifstream stream(path);
	string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(string s) {
	for(char c : s) {
		if(std::isalpha(c)) {
			if(std::islower(c)) {
				c = std::toupper(c);
			} else {
				c = std::tolower(c);
			}
		}
		cout << c;
	}
	cout << endl;
}
