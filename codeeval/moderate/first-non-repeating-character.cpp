/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
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
	process("yellow");
	std::cout << "y\n";// std::endl for flush?
	process("tooth");
	std::cout << "h\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	if(s.empty()) {
		return;
	}
	using std::begin;
	using std::end;
	while(std::count(begin(s), end(s), s[0]) > 1 ) {
		char c = s.front();
		size_t ofs = 0;
		while(std::string::npos != (ofs = s.find(c, ofs))) {
			s.erase(ofs, 1);
		}
	}
	std::cout << s[0] <<"\n";
}
