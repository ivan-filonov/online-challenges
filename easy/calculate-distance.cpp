/*
 * Common base C++ source for codeeval solutions.
 * */
#include <cmath>
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
	process("(25, 4) (1, -6)");
	std::cout << "26" << std::endl;
	process("(47, 43) (-25, -11)");
	std::cout << "90" << std::endl;
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
	int x1, x2, y1, y2;
	ss.ignore(10,'(');
	ss >> x1;
	ss.ignore(10,' ');
	ss >> y1;
	ss.ignore(10,'(');
	ss >> x2;
	ss.ignore(10,' ');
	ss >> y2;
	x2 -= x1;
	y2 -= y1;
	std::cout << (int)std::sqrt(x2*x2 + y2*y2) << std::endl;
}
