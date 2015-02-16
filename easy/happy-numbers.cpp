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
	process("1");
	std::cout << "1" << std::endl;
	process("7");
	std::cout << "1" << std::endl;
	process("22");
	std::cout << "0" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

template<typename T>
T step(T n) {
	T res = 0;
	while(n > 0) {
		auto nn = n % 10;
		res += nn * nn;
		n /= 10;
	}
	return res;
}

void process(std::string s) {
	auto n = std::stoi(s);
	auto n1 = step(n);
	while(n1 != 1) {
		if(n == n1) {
			break;
		}
		n = step(n);
		n1 = step(step(n1));
	}
	std::cout << (n1 == 1) << std::endl;
}
