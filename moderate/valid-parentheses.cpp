/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("()");
	std::cout << "True" << std::endl;
	process("([)]");
	std::cout << "False" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
//	std::cout << "TODO: procees line " << s << std::endl;
	std::vector<char> ps;
	bool fail = false;
	for(char c : s) {
		if('('==c||'['==c||'{'==c) {
			ps.push_back(c);
		} else if(')' == c) {
			if(ps.empty() || ps.back() != '(') {
				fail = true;
				break;
			}
			ps.pop_back();
		} else if(']' == c) {
			if(ps.empty() || ps.back() != '[') {
				fail = true;
				break;
			}
			ps.pop_back();
		} else if('}' == c) {
			if(ps.empty() || ps.back() != '{') {
				fail = true;
				break;
			}
			ps.pop_back();
		}
	}
	fail |= !ps.empty();
	std::cout << (fail ? "False" : "True") << std::endl;
}
