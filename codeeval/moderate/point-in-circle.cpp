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
	process("Center: (2.12, -3.48); Radius: 17.22; Point: (16.21, -5)");
	std::cout << "true" << std::endl;
	process("Center: (5.05, -11); Radius: 21.2; Point: (-31, -45)");
	std::cout << "false" << std::endl;
	process("Center: (-9.86, 1.95); Radius: 47.28; Point: (6.03, -6.42)");
	std::cout << "true" << std::endl;
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
	double cx,cy, r, px, py;

	ss.ignore(100, '(');
	ss >> cx;

	ss.ignore(100, ',');
	ss >> cy;

	ss.ignore(100, ':');
	ss >> r;

	ss.ignore(100, '(');
	ss >> px;

	ss.ignore(100, ',');
	ss >> py;

	cx -= px;
	cy -= py;

	std::cout << (( r*r > (cx*cx + cy*cy) ) ? "true" : "false") << std::endl;
}
