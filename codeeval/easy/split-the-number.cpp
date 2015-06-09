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
	process("3413289830 a-bcdefghij");
	std::cout << "-413289827" << std::endl;

	process("776 a+bc");
	std::cout << "83" << std::endl;

	process("12345 a+bcde");
	std::cout << "2346" << std::endl;

	process("1232 ab+cd");
	std::cout << "44" << std::endl;

	process("90602 a+bcde");
	std::cout << "611" << std::endl;
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
	long res = 0, cur = 0, sgn = 1;
	auto it_dig = std::begin(s);
	auto it_ctl = it_dig + s.find(' ') + 1;
	while(std::end(s) != it_ctl) {
		if('-' == *it_ctl) {
			res += cur * sgn;
			cur = 0;
			sgn = -1;
		} else if( '+' == *it_ctl) {
			res += cur * sgn;
			cur = 0;
			sgn = 1;
		} else {
			cur = cur * 10 + ((*it_dig++) - '0');
		}
		++it_ctl;
	}
	res += cur * sgn;
	std::cout << res << std::endl;
}
