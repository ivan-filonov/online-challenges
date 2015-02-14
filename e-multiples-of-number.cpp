#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void process(string s) {
	size_t delim;
	auto x = std::stoi(s, &delim);
	auto n = std::stoi(s.substr(delim + 1));
	auto m = n;
	while((m << 1) < x) {
		m <<= 1;
	}
	while(m < x) {
		m += n;
	}
	cout << m << endl;
}

int main(int _a, char ** _v) {
	ifstream fs(_v[1]);
	string s;
	while(std::getline(fs, s)) {
		process(s);
	}
}
