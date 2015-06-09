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
	process("1 2 3 4 5 6 7 8 9 : 0-8");
	std::cout << "9 2 3 4 5 6 7 8 1" << std::endl;
	process("1 2 3 4 5 6 7 8 9 10 : 0-1, 1-3");
	std::cout << "2 4 3 1 5 6 7 8 9 10" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::vector<int> v;
	std::istringstream ss(s);
	while(!ss.eof()) {
		std::string p;
		ss >> p;
		if(p == ":") {
			break;
		}
		v.push_back(std::stoi(p));
	}

	while(!ss.eof()) {
		std::string p;
		ss.ignore(1, ' ');
		std::getline(ss, p, ',');
		size_t x;
		auto i1 = std::stoi(p,&x);
		auto i2 = std::stoi(p.substr(x + 1));
		std::swap(v.at(i1), v.at(i2));
	}

	for(auto b = std::begin(v), it = b; std::end(v) != it; ++it) {
		if(b != it) {
			std::cout << ' ';
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}
