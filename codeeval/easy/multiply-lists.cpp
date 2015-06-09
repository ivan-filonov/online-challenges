/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <sstream>
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
	process("9 0 6 | 15 14 9");
	cout << "135 0 54" << endl;
	process("5 | 8");
	cout << "40" << endl;
	process("13 4 15 1 15 5 | 1 4 15 14 8 2");
	cout << "13 16 225 14 120 10" << endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(string s) {
	auto split = s.find('|');
	std::istringstream s1(s.substr(0, split));
	std::istringstream s2(s.substr(split + 1));
	bool mid = false;
	while(!s1.eof() && !s2.eof()) {
		int a,b;
		s1 >> a;
		s2 >> b;
		if(mid) {
			cout << ' ';
		} else {
			mid = true;
		}
		cout << a * b;
	}
	cout << endl;
}
