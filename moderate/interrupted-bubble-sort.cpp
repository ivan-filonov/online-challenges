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
	return 1;
}

void process(std::string s);

void test() {
	process("36 47 78 28 20 79 87 16 8 45 72 69 81 66 60 8 3 86 90 90 | 1");
	std::cout << "36 47 28 20 78 79 16 8 45 72 69 81 66 60 8 3 86 87 90 90\n";// std::endl for flush?
	process("40 69 52 42 24 16 66 | 2");
	std::cout << "40 42 24 16 52 66 69\n";// std::endl for flush?
	process("54 46 0 34 15 48 47 53 25 18 50 5 21 76 62 48 74 1 43 74 78 29 | 6");
	std::cout << "0 15 25 18 34 5 21 46 47 48 48 1 43 50 53 29 54 62 74 74 76 78\n";// std::endl for flush?
	process("48 51 5 61 18 | 2");
	std::cout << "5 48 18 51 61\n";// std::endl for flush?
	process("59 68 55 31 73 4 1 25 26 19 60 0 | 2");
	std::cout << "55 31 59 4 1 25 26 19 60 0 68 73\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	if(s.empty()) {
		return;
	}
	std::istringstream ss(s);

	typedef long long num_t;
	auto conv = [](const std::string& str){return std::stoll(str);};
	
	std::vector<num_t> v;
	std::string n;
	while(std::getline(ss, n, ' ')) {
		if(n == "|") {
			break;
		}
		v.push_back(conv(n));
	}
	std::getline(ss, n, ' ');
	auto steps = conv(n);

	using std::begin;
	using std::end;
	
	for(int i  = 0; i < steps ; ++i) {
		bool stop = true;
		for(auto it = begin(v), jt = it + 1; end(v) != jt; ++it, ++jt) {
			if(*it > *jt) {
				auto t = *it;
				*it = *jt;
				*jt = t;
				stop = false;
			}
		}
		if(stop) {
			break;
		}

	}
	
	for(auto b = std::begin(v), it = b, e = std::end(v); e != it; ++it) {
		if(b != it) {
			std::cout << ' ';
		}
		std::cout << *it;
	}
	std::cout << "\n";
}
