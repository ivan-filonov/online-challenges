/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

void process_file(char*);
void process(std::string s);
int main(int _a, char ** _v) {
	process_file(_v[1]);
	return 0;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::istringstream ss(s);
	std::vector<std::string> tok;
	typedef std::istream_iterator<std::string> _iter_t;
	std::copy(_iter_t(ss), _iter_t(), std::back_inserter(tok));
	std::reverse(tok.begin(), tok.end());
	std::vector<double> stack;
	for(auto t : tok) {
		int v;
		if(std::isdigit(t[0])) {
			stack.push_back( std::stoi(t) );
//			std::cout << "push " << stack.back() << std::endl;
		} else {
			auto a = stack.back();
			stack.pop_back();
//			std::cout << "pop -> " << a << std::endl;
			switch(t[0]) {
				case '+':
					stack.back() += a;
					break;
				case '*':
					stack.back() *= a;
					break;
				case '/':
					stack.back() = a / stack.back();
					break;
			}
//			std::cout << "op = " << t << ", result = " << stack.back() << std::endl;
		}
	}
	std::cout << (long) stack.front() << std::endl;
}
