/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
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
	process("ABbCcc");
	std::cout << "152" << std::endl;
	process("Good luck in the Facebook Hacker Cup this year!");
	std::cout << "754" << std::endl;
	process("Ignore punctuation, please :)");
	std::cout << "491" << std::endl;
	process("Sometimes test cases are hard to make up.");
	std::cout << "729" << std::endl;
	process("So I just go consult Professor Dalves");
	std::cout << "646" << std::endl;
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
	std::vector<int> cc(26, 0);
	for(char c : s) {
		if(std::isalpha(c)){
			c = std::tolower(c) - 'a';
			++cc.at(c);
		}
	}
	std::sort(cc.begin(), cc.end());
//	std::cout << cc.front() << ' ' << cc.back() << std::endl;
	int sum = 0;
	for(int i = 0; i < 26; ++i) {
		sum += cc[i] * (i + 1);
	}
	std::cout << sum << std::endl;
}
