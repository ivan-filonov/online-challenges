#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
void process(const string &s) {
//	cout << s << endl;
	istringstream iss(s);
	int d1,d2,n;
	iss >> d1 >> d2 >> n;
//	cout << d1 << ", " << d2 << ", " << n << endl;
	for(int i = 1, f = 0; i <=n; ++i) {
		if(f) {
			cout << ' ';
		} else {
			f = 1;
		}
		auto r1 = 0==(i%d1);
		auto r2 = 0==(i%d2);
		if(!r1 && !r2) {
			cout << i;
		} else {
			string s{""};
			if(r1) s += 'F';
			if(r2) s += 'B';
			cout << s;
		}
	}
	cout << endl;
}
void test() {
	process("3 5 10");
	cout << "1 2 F 4 B F 7 8 F B" << endl;
}
int main(int argc, char *argv[]) {
//	test();
	ifstream stream(argv[1]);
	string line;
	while (getline(stream, line)) {
		// Do something with the line
		if(line.empty()) {
			continue;
		}
		process(line);
	}
	return 0;
}
