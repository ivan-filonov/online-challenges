/*
 * Common base C++ source for codeeval solutions.
 * */
#include <cctype>
#include <cmath>
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
	process("a b c d");
	std::cout << "c a d b" << std::endl;
	process("a b c d e f g h i j k l m n o p");
	std::cout << "m i e a n j f b o k g c p l h d" << std::endl;
	process("a b c d e f g h i");
	std::cout << "g d a h e b i f c" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::vector<char> mat;
	mat.reserve(100);
	for(char c: s) {
		if(std::isalpha(c)) {
			mat.push_back(c);
		}
	}
	auto n = (int)std::sqrt(mat.size());
	std::vector<char> m2(n*n);
	for(int v = 0; v < n; ++v) {
		for(int h = 0; h < n; ++h) {
			auto h1 = v;
			auto v1 = n - h - 1;
			m2[v*n + h] = mat[v1 *n + h1];
		}
	}
	for(auto it = m2.begin(); m2.end() != it; ++it) {
		if(m2.begin() != it) {
			std::cout << ' ';
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}
