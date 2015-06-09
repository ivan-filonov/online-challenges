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
	process("8,33,21,0,16,50,37,0,melon,7,apricot,peach,pineapple,17,21");
	std::cout << "melon,apricot,peach,pineapple|8,33,21,0,16,50,37,0,7,17,21" << std::endl;
	process("24,13,14,43,41");
	std::cout << "24,13,14,43,41" << std::endl;
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
	std::vector<int> numbers;
	std::vector<std::string> words;
	std::istringstream ss(s);
	std::string part;
	while(std::getline(ss, part, ',')) {
		if(std::isdigit( *std::begin(part) ) ) {
			numbers.push_back(std::stoi(part));
		} else {
			words.push_back(part);
		}
	}
	for(auto it = std::begin(words), b = it ;std::end(words) != it; ++it) {
		if(b != it) {
			std::cout << ',';
		}
		std::cout << *it;
	}
	if(!words.empty() && !numbers.empty()) {
		std::cout << '|';
	}
	for(auto it = std::begin(numbers), b = it ;std::end(numbers) != it; ++it) {
		if(b != it) {
			std::cout << ',';
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}
