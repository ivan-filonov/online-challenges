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
	process_file(_v[1]);
}

void process(string s);

void test() {
	process("some line with text");
	process("another line");
}

void process_file(char* path) {
	std::ifstream stream(path);
	string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(string s) {
//	cout << "TODO: procees line " << s << endl;
	auto ls = s.rfind(' ');
	auto ls1 = s.rfind(' ', ls - 1);
	if(string::npos == ls1) {
		ls1 = 0;
	} else {
		++ls1;
	}
	cout << s.substr(ls1,ls-ls1) << endl;
}
