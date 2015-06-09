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

typedef long num_t;
typedef std::vector<num_t> vec_t;

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

vec_t state;

void test() {
	process("5");
	process("9 6");
	process("4 6 8");
	process("0 7 1 5");
	std::cout << *std::max_element(state.begin(), state.end()) << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	state.clear();
	state.reserve(1024);
	while(std::getline(stream, line)) {
		process(line);
	}
	std::cout << *std::max_element(state.begin(), state.end()) << std::endl;
}

vec_t expand(vec_t v) {
	vec_t res;
	res.reserve(1024);
	res.resize(v.size() + 1);
	std::copy(v.begin(), v.end(), res.begin());
	res.back() = 0;
	for(auto src = v.begin(), dst = res.begin() + 1, end = v.end(); end != src; ++src, ++dst) {
		*dst = std::max(*src, *dst);
	}
	return res;
}

void process(std::string s) {
	state = expand(state);
	std::istringstream ss(s);
	vec_t line;
	line.reserve(1024);
	typedef std::istream_iterator<num_t> _iter_t;
	std::copy(_iter_t(ss), _iter_t(), std::back_inserter(line));
	for(auto src = line.begin(), dst = state.begin(), end = line.end(); end != src; ++src, ++dst) {
		*dst += *src;
	}
}
