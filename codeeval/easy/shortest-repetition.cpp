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
	process("abcabcabcabc");
	std::cout << "3" << std::endl;
	process("bcbcbcbcbcbcbcbcbcbcbcbcbcbc");
	std::cout << "2" << std::endl;
	process("dddddddddddddddddddd");
	std::cout << "1" << std::endl;
	process("adcdefg");
	std::cout << "7" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	size_t len = s.length();
	size_t l = 0;
	for(size_t l = 1; l <= len/2; ++l) {
		bool eq = false;
		for(size_t o = 0; (!eq) && (o <= len - 2 * l); ++ o) {
			eq = true;
			for(size_t i = o, j = o + l; eq && (i < o+l); ++i, ++j) {
				eq = s[i] == s[j];
			}
		}
		if(eq) {
			len = l;
			break;
		}
	}
	std::cout << len << std::endl;
}
