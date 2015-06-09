/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <sstream>
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
	process("5;0,1,2,3,0");
	std::cout << "0\n";
	process("20;0,1,10,3,2,4,5,7,6,8,11,9,15,12,13,4,16,18,17,14");
	std::cout << "4\n";
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
	std::istringstream ss(s);
	
	int n;
	ss >> n;
	ss.ignore(10,';');
	
	std::string ns;
	int sum = 0;
	int cnt = 0;
	while(std::getline(ss, ns, ',')) {
		sum += std::stoi(ns);
		++cnt;
	}
	std::cout << (sum - (cnt-1)*(cnt-2)/2) << "\n";
}
