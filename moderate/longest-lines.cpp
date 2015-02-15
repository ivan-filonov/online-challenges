/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void process_file(char*);
int main(int _a, char ** _v) {
	process_file(_v[1]);
	return 0;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	std::getline(stream, line);
	auto os = std::stoi(line);
	std::vector<std::string> vec;
	while(std::getline(stream, line)) {
		vec.push_back(line);
		std::sort(vec.rbegin(), vec.rend(), 
			[](const std::string& a, const std::string& b) {
				return a.length() > b.length();
			} );
		if(vec.size() > os) {
			vec.pop_back();
		}
	}
	bool mid = false;
	for(auto s : vec) {
		std::cout << s << std::endl;
	}
}

