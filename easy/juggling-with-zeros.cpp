/*
 * Common base C++ source for codeeval solutions.
 * */
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
	process("00 0 0 00 00 0");
	process("00 0");
	process("00 0 0 000 00 0000000 0 000");
	process("0 000000000 00 00");

	std::cout << "9" << std::endl
		<< "1" << std::endl
		<< "9208" << std::endl
		<< "3" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	int ofs = 0;
	long v = 0;
	bool flag = false;
	while(ofs < s.length()) {
		// 1. get flag
		while(' ' == s[ofs]) ++ofs;
		flag = '0' == s[ofs+1];
		if(flag) {
			++ofs;
		}
		++ofs;
		// 2. process digits
		while(' ' == s[ofs]) ++ofs;
		while('0' == s[ofs]) {
			++ofs;
			v <<= 1;
			if(flag) {
				++v;
			}
		}
	}
	std::cout << v << std::endl;
}
