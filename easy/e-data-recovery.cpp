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
using namespace std;

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
}

void process(string s);

void test() {
	process("2000 and was not However, implemented 1998 it until;9 8 3 4 1 5 7 2");
	cout << "However, it was not implemented until 1998 and 2000" << endl;
	process("programming first The language;3 2 1");
	cout << "The first programming language" << endl;
	process("programs Manchester The written ran Mark 1952 1 in Autocode from;6 2 1 7 5 3 11 4 8 9");
	cout << "The Manchester Mark 1 ran programs written in Autocode from 1952" << endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(string s) {
//	cout << "TODO: procees line " << s << endl;
	auto delim = s.rfind(';');
	auto wordlist = s.substr(0, delim);
	auto numlist = s.substr(delim + 1);
//	cout << "words: { " << wordlist << " }, nums: { " << numlist << " }" << endl;
	
	vector<string> words;
	size_t ofs = 0;
	while(ofs < wordlist.length()) {
		auto delim = wordlist.find(' ', ofs);
		if(string::npos == delim) {
			delim = wordlist.length();
		}
		words.push_back(wordlist.substr(ofs, delim - ofs));
		ofs = delim + 1;
	}

	vector<int> idx(words.size(), -1);
	int i = 0;
	std::istringstream nums(numlist);
	while(!nums.eof()) {
		int num;
		nums >> num;
		idx[num - 1] = i;
		++i;
	}

	for(auto iter = idx.begin(), end = idx.end(); end != iter; ++iter) {
		if(idx.begin() != iter) {
			cout << ' ';
		}
		if(-1 == *iter) {
			for(int i = 0; i < words.size(); ++i) {
				if(std::find(idx.begin(), idx.end(), i) == idx.end()) {
					cout << words[i];
				}
			}
		} else {
			cout << words[*iter];
		}
	}
	cout << endl;
}
