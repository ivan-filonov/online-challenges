#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
using namespace std;
void fib(long n) {
	if(!n) {
		cout << 0 << endl;
		return;
	}
//	cout << "fib(" << n << ")" << endl;
	long long a = 1, b = 1;
	while(n-->1) {
		tie(a,b)=make_tuple(b,a+b);
	}
	cout << a << endl;
}

int main(int argc, char *argv[]) {
	ifstream stream(argv[1]);
	string line;
	while (getline(stream, line)) {
		// Do something with the line
		if(line.empty()) {
			continue;
		}
		auto k = stol(line);
		fib(k);
	}
	return 0;
}
