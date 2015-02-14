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
}

void process(std::string s);

void test() {
	process("10");
	std::cout << "2,3,5,7" << std::endl;
	process("20");
	std::cout << "2,3,5,7,11,13,17,19" << std::endl;
	process("100");
	std::cout << "2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

namespace {
	long lt = 3;
	std::vector<long> primes {2, 3};
}

void process(std::string s) {
	auto lim = std::stol(s);
//	std::cout << "lim = " << lim << std::endl;
	while(lt < lim) {
		lt += 2;
		bool prime = true;
		for(long p : primes) {
			if(0 == lt%p) {
				prime = false;
				break;
			}
		}
		if(prime) {
			primes.push_back(lt);
		}
	}
	bool mid = false;
	for(long p : primes) {
		if(p >= lim) {
			break;
		}
		if(mid) {
			std::cout << ',';
		} else {
			mid = true;
		}
		std::cout << p;
	}
	std::cout << std::endl;
}
