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
	process("1,1,1,2,2,3,3,4,4");
	std::cout << "1,2,3,4\n";// std::endl for flush?
	process("2,3,4,5,5");
	std::cout << "2,3,4,5\n";// std::endl for flush?
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
	std::string n;
	bool mid = false;
	bool first = true;
	int prev;
	while(std::getline(ss, n, ',')) {
		auto i = std::stoi(n);
		if(first) {
			prev = i;
			first = false;
		}
		if(prev == i) {
			continue;
		}
		if(mid) {
			std::cout << ',';
		} else {
			mid = true;
		}
		std::cout << prev;
		prev = i;
	}
	std::cout << (mid?",":"") << prev <<  "\n";
}
