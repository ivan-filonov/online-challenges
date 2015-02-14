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
	process("1 2 3 4");
	std::cout << "4 2" << std::endl;
	process("10 -2 3 4");
	std::cout << "4 -2" << std::endl;
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
	typedef std::istream_iterator<std::string> _iter_t;
	std::copy(_iter_t(ss),_iter_t(),std::back_inserter(words));
	for(auto begin = words.rbegin(), end = words.rend(), iter = begin; end != iter; ) {
		if(iter != begin) {
			std::cout<<' ';
		}
		std::cout << *iter;
		
		if(end == ++iter) {
			break;
		}
		++iter;
	}
	std::cout << std::endl;
}
