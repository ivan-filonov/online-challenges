/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
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
	process("1,2,3,4,5;2");
	std::cout << "2,1,4,3,5" << std::endl;
	process("1,2,3,4,5;3");
	std::cout << "3,2,1,4,5" << std::endl;
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
	std::vector<int> nums;
	int gs;
	std::istringstream ss(s);
	int n;
	char c;
	do {
		ss >> n >> c;
		nums.push_back(n);
	} while (';' != c);
	ss >> gs;
	for(int i = 0; i < nums.size() / gs; ++i) {
		std::reverse(std::begin(nums) + i * gs, std::begin(nums) + i * gs + gs);
	}
	for(auto b = std::begin(nums), it = b, e = std::end(nums); e != it; ++it) {
		if(b != it) {
			std::cout << ',';
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}
