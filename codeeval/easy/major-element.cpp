/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("92,19,19,76,19,21,19,85,19,19,19,94,19,19,22,67,83,19,19,54,59,1,19,19");
	std::cout << "19" << std::endl;
	process("92,11,30,92,1,11,92,38,92,92,43,92,92,51,92,36,97,92,92,92,43,22,84,92,92");
	std::cout << "92" << std::endl;
	process("4,79,89,98,48,42,39,79,55,70,21,39,98,16,96,2,10,24,14,47,0,50,95,20,95,48,50,12,42");
	std::cout << "None" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::map<int,int> map;
	int count = 0;
	std::istringstream ss(s);
	while(!ss.eof()) {
		char comma;
		int value;
		ss >> value >> comma;
		auto it = map.find(value);
		if(std::end(map) == it) {
			map[value] = 1;
		} else {
			++it->second;
		}
		++count;
	}
	count /= 2;
	auto it = std::find_if(std::begin(map), std::end(map), [=](const std::pair<int,int>& p) {
		return p.second > count;
	});
	if(std::end(map) == it) {
		std::cout << "None" << std::endl;
	} else {
		std::cout << it->first  << std::endl;
	}
}
