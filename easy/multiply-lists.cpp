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
	test();
	process_file(_v[1]);
}

void process(string s);

void test() {
	process("");
	cout << "" << endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(string s) {
	cout << "TODO: procees line " << s << endl;
}
