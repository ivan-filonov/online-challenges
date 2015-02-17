/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
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
	process("A quick brown fox jumps over the lazy dog");
	std::cout << "NULL\n";// std::endl for flush?
	process("A slow yellow fox crawls under the proactive dog");
	std::cout << "bjkmqz\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	int m = (1 << 26) - 1;
	for(auto c : s) {
		if(!std::isalpha(c)) {
			continue;
		}
		m &= ~(1 << (std::tolower(c) - 'a'));
	}
	if(m) {
		for(int i = 0; i < 26; ++i) {
			if(m & (1<<i)) {
				std::cout << (char)('a' + i);
			}
		}
	} else {
		std::cout << "NULL";
	}
	std::cout << std::endl;
}
