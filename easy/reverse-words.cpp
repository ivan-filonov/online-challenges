/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
}

void process(std::string s);

void test() {
	process("Hello World");
	std::cout << "World Hello" << std::endl;
	process("Hello CodeEval");
	std::cout << "CodeEval Hello" << std::endl;
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
	std::vector<std::string> words;
	std::copy(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(), std::back_inserter(words));
	std::reverse(words.begin(), words.end());
	for(auto iter = words.begin(), end = words.end(); end != iter; ++iter) {
		if(words.begin() != iter) {
			std::cout << ' ';
		}
		std::cout << *iter;
	}
	std::cout << std::endl;
}
