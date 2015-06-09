#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void process(const string &line) {
	auto ps = line.find_first_of(',');
	decltype(ps) cpos = 0;
	decltype(ps) res  = line.length();
	while(cpos < line.length()) {
		auto sub = line.substr(cpos, ps);
		cpos += ps + 1;
		
		res = std::min(res,sub.find_first_of('Y') - sub.find_last_of('X') - 1 );
	}
	cout << res << endl;
}

int main(int _a, char ** _v) {
/*
	process("XX.YY,XXX.Y,X..YY,XX..Y");
	process("XX...YY,X....YY,XX..YYY,X..YYYY");
	process("XXYY,X..Y,XX.Y");
*/
	ifstream stream(_v[1]);
	string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}
