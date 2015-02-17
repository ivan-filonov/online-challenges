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
	process("2,10");
	std::cout << "4" "\n";
	process("20,30");
	std::cout << "2" "\n";
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

	long count_to(long lim) {
		long res = 0;
		while(lt <= lim) {
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
				++res;
			}
		}
		return res;
	}
}

void process(std::string s) {
	auto from = std::stoi(s);
	s.erase(0, s.find(',') + 1);
	auto to = std::stoi(s);
	count_to(to);
	int res = 0;
	for(auto b = std::begin(primes), it = b, e = std::end(primes); e != it; ++it) {
		if(from > *it) {
			continue;
		}
		if(to < *it) {
			break;
		}
		++res;
	}
	std::cout << res << "\n";
}
