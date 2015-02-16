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
	process("10, 3");
	std::cout << "2 5 8 1 6 0 7 4 9 3" << std::endl;
	process("5, 2");
	std::cout << "1 3 0 4 2" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	size_t o;
	std::vector<int> nums(std::stoi(s, &o));
	for(int i = 0; i < nums.size(); ++i) {
		nums[i] = i;
	}
	auto p = std::stoi(s.substr(o+1));
	auto it = std::begin(nums);
	bool mid = false;
	while(!nums.empty()) {
		if(mid) {
			std::cout << ' ';
		} else {
			mid = true;
		}
		for(int i = 1; i < p; ++i) {
			++it;
			if(std::end(nums) == it) {
				it = std::begin(nums);
			}
		}
		std::cout << * it;
		it = nums.erase(it);
		if(std::end(nums) == it) {
			it = std::begin(nums);
		}
	}
	std::cout << std::endl;
}
