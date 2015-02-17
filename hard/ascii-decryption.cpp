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
	process("5 | s | 92 112 109 40 118 109 109 108 123 40 119 110 40 124 112 109 40 117 105 118 129 40 119 125 124 127 109 113 111 112 40 124 112 109 40 118 109 109 108 123 40 119 110 40 124 112 109 40 110 109 127 54 40 53 40 91 120 119 107 115 123 40");
	std::cout << "The needs of the many outweigh the needs of the few. - Spock"  "\n";// std::endl for flush?
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
	std::string t;
	std::getline(ss, t, ' ');// word length
	const auto wl = std::stoi(t);
	std::getline(ss, t, ' ');// |
	std::getline(ss, t, ' ');// last char of that word
	const int kc = t[0];
	std::getline(ss, t, ' ');// |
	std::vector<int> v;
	while(std::getline(ss, t, ' ')) {
		v.push_back(std::stoi(t));
	}
	const int spc_diff = kc - ' ';
	std::vector<int> words;//word candidates
	for(int i = 0, e = v.size() - wl; e != i; ++i) {
		bool cond = (spc_diff == v[i+wl-1] - v[i+wl]);
		if(i) {
			cond &= spc_diff == v[i+wl-1] - v[i-1];
		}
		if(cond) {
			words.push_back(i);
		}
	}
	//find the first pair of same words
	int pw = -1;
	for(int i = 0; i < words.size() - 1; ++i) {
		for(int j = i + 1; j < words.size(); ++j) {
			auto b = std::begin(v);
			if(std::equal(b + words[i], b + words[i] + wl, b + words[j])) {
				pw = i;
				break;
			}
		}
	}
	pw = words[pw];
	int d = v[(pw > 0) ? (pw - 1) : (pw + wl)] - ' ';
	auto fn = [=](int x){std::cout << (char)(x-d);};
	std::for_each(std::begin(v), std::end(v), fn);
	std::cout << "\n";
}
