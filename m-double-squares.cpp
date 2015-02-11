#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;
void process_line(string s) {
//	cout << s << endl;
	auto n = stol(s);
//	auto nr = static_cast<decltype(n)>(std::sqrt(n));
//	cout << n << " -> " << nr << endl;

	auto b =  static_cast<decltype(n)>(std::sqrt(n));
	auto a = 0;
	int count = 0;
	while(b >= a) {
		auto v = a*a+b*b;
		if(v==n) {
			++count;
//			cout << a << "^2 + " << b << "^2 = " << n << endl;
			--b;
			++a;
		} else {
			if(v > n) {
				--b;
				a = std::sqrt(n-b*b);
			} else {
				++a;
				b = std::sqrt(n-a*a);
			}
		}
	}
	cout << count << endl;
}
void test() {
	auto t = {10, 25, 3, 0, 1};
	auto r = {1, 2, 0, 1, 1};
	for(auto si=t.begin(), di=r.begin(); t.end() != si; ++si, ++di) {
		process_line(to_string(*si));
		cout << *di << endl;
	}
}
int main(int _c,char**_v) {
//	test();
	ifstream stream(_v[1]);
	string line;
	int c = -1;
	while(std::getline(stream,line)){
		if(-1==c) {
			c = std::stoi(line);
		}  else {
			process_line(line);
			if(!--c) {
				break;
			}
		}
	}
	return 0;
}
