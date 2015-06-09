/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <set>
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
	process("1,2,3,4;4,5,6");
	std::cout << "4" << std::endl;
	process("20,21,22;45,46,47");
	std::cout << "" << std::endl;
	process("7,8,9;8,9,10,11,12");
	std::cout << "8,9" << std::endl;
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
	int v;
	char c;
	std::set<int> s1, s2;
	auto ptr = &s1;
	while(!ss.eof()) {
		ss >> v >> c;
		ptr->insert(v);
		if(';' == c) {
			ptr = &s2;
		}
	}
	bool mid = false;
	for(auto it=s1.begin(), e=s1.end(); e != it; ++it) {
		if(s2.end() == s2.find(*it)) {
			continue;
		}
		if(mid) {
			std::cout << ',';
		} else {
			mid = true;
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}
