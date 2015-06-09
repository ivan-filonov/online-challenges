/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("4-2;BEGIN-3;3-4;2-END");
	std::cout << "GOOD"  "\n";// std::endl for flush?
	process("4-2;BEGIN-3;3-4;2-3");
	std::cout << "BAD"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

const int c_end = std::numeric_limits<int>::max();
const int c_begin = c_end - 1;

int token(std::string& s, size_t& from) {
	char c = s[from];
	if('-' == c || ';' == c ) {
		++from;
		c = s[from];
	}
	if('B' == c) {
		if(s.substr(from,5) == "BEGIN") {
			from += 5;
			return c_begin;
		}
	} else if('E' == c) {
		if(s.substr(from,3) == "END") {
			from += 3;
			return c_end;
		}
	} else if(std::isdigit(c)) {
		int res = 0;
		while(std::isdigit(c = s[from++])) {
			res = res * 10 + c - '0';
		}
		return res;
	}
	std::cout << s.substr(from) << "\n";
	throw s;
}

void process(std::string s) {
	size_t pos = 0;
	std::map<int,int> map;
	while(pos < s.length()) {
		const int a = token(s, pos);
		const int b = token(s, pos);
		map[a] = b;
	}
	int v = c_begin, n = map.size();
	bool fail = false;
	while(n--) {
		if(map.end() ==  map.find(v)) {
			fail = true;
			break;
		}
		v = map[v];
	}
	if(!fail) {
		fail = (v != c_end);
	}
	std::cout << (fail ? "BAD" : "GOOD") << "\n";
}
