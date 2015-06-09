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

static char board[65536];

void process(std::string s) {
	std::istringstream ss(s);
	std::string w;
	int coord, val;
	ss >> w;
	if(w == "SetCol") {
		ss >> coord >> val;
		for(int i = 0, j = coord; i < 256; ++i, j+=256) {
			board[j] = val;
		}
	} else if (w == "SetRow") {
		ss >> coord >> val;
		for(int i = 0, j = coord * 256; i < 256; ++i, ++j) {
			board[j] = val;
		}
	} else if (w == "QueryCol") {
		ss >> coord;
		int res = 0;
		for(int i = 0, j = coord; i < 256; ++i, j +=256) {
			res += board[j];
		}
		std::cout << res << std::endl;
	} else if (w == "QueryRow") {
		ss >> coord;
		int res = 0;
		for(int i = 0, j = 256*coord; i < 256; ++i, ++j) {
			res += board[j];
		}
		std::cout << res << std::endl;
	}
}
