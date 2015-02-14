#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;

void process(string line) {
	vector<char> chars;
	size_t delim = 999;
	auto sl = stoi(line, &delim);
//	cout << "strings length: " << sl << endl;
//	cout << "charset source: " << line.substr(delim+1) << endl;
	for(char c : line.substr(delim+1)) {
		if(chars.end() == std::find(chars.begin(), chars.end(), c)) {
			chars.push_back(c);
		}
	}
	std::sort(chars.begin(), chars.end());
//	cout << "charset: length = " << chars.size() << ", chars: ";
//	for(auto iter = chars.begin(), end = chars.end(); end != iter; ++iter) {
//		if(chars.begin() != iter) {
//			cout << ", ";
//		}
//		cout << *iter;
//	}
//	cout << endl;
	long tc = 1;
	for(int n = sl; n > 0; --n) {
		tc *= chars.size();
	}
//	cout << "total results: " << tc << endl;
	int base = static_cast<int>(chars.size());
	string s;
	for(int i = 0 ; i < tc; ++i) {
		if(i) {
			cout << ',';
		}
		s.clear();
		for(int j = 0, v = i; j < sl; ++j) {
			s += chars[v%base];
			v /= base;
		}
		std::reverse(s.begin(), s.end());
		cout << s;
	}
	cout << endl;
}

void test() {
	process("1,aa");
	cout << "a" << endl;
	process("2,ab");
	cout << "aa,ab,ba,bb" << endl;
	process("3,pop");
	cout << "ooo,oop,opo,opp,poo,pop,ppo,ppp" << endl;
}

int main(int _a, char ** _v) {
//	test();
	ifstream stream(_v[1]);
	string s;
	while(std::getline(stream,s)) {
		process(s);
	}
}

