#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
void cmyk(string s) {
	istringstream ss {s};
	char z;
	double c,m,y,k;
	ss >> z  >> c >> z >> m >> z >> y >> z >> k >> z;
	auto coeff = 255.0 * (1.0-k);
	cout << lround( coeff * (1 - c)) << ',' << lround(coeff * (1-m)) << ',' << lround(coeff*(1-y));
}

void hex(string s) {
	auto v = std::stoi(s.substr(1), nullptr, 16);
	cout << (255&(v>>16)) << ',' << (255&(v>>8)) << ',' << (255&v);
}

void hsl(string s) {
	istringstream ss {s};
	char z;
	double h,s,l;
	ss.ignore(4);
	ss >> h >> z >> s >> z >> l >> z;
	s *= .01;
	l *= .01;
}

void hsv(string s) {
	istringstream ss {s};
	char z;
}

void process(string line) {
	cout << "RGB(";
	if('(' == line[0]) {
		cmyk(line);
	}
	else if('#' == line[0]) {
		hex(line);
	}
	else if('H' == line[0]) {
		if('L' == line[2]) {
			hsl(line);
		}
		else if('V' == line[2]) {
			hsv(line);
		}
		else throw "bad arg";
	}
	else throw "bad arg";
	cout << ')' << endl;
}

int main() {
	process("(0.56,0.94,0.21,0.02)");
	process("HSL(359,0,0)");
	process("HSV(276,33,7)");
	process("#cfa9c4");
}

