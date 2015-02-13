#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
const string alpha { " !\"#$%&'()*+,-./0123456789:<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" };

void process(string line) {
	vector<char> key;
	string msg;
	auto split = line.find(';');
	std::copy(line.begin(), line.begin() + split, std::back_inserter(key));
	std::transform(key.begin(), key.end(), key.begin(), [](char c){
		return  alpha.length() - (c - '0');
	});
	msg = line.substr(split + 1);
	for(int i = 0; i < msg.length(); ++i) {
		auto c = msg[i];
		cout << alpha[(alpha.find(c) + key[i%key.size()]) % alpha.length()];
	}
	cout << endl;
}

void test() {
	process("31415;HYEMYDUMPS");
	cout << "EXALTATION" << endl;
	process("45162;M%muxi%dncpqftiix\"");
	cout << "I love challenges!" << endl;
	process("14586214;Uix!&kotvx3");
	cout << "Test input." << endl;
}

int main(int _a, char ** _v) {
	// test();

	ifstream stream(_v[1]);
	string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

