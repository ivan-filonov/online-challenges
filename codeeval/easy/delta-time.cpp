/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("14:01:57 12:47:11");
	std::cout << "01:14:46" << std::endl;
	process("13:09:42 22:16:15");
	std::cout << "09:06:33" << std::endl;
	process("08:08:06 08:38:28");
	std::cout << "00:30:22" << std::endl;
	process("23:35:07 02:49:59");
	std::cout << "20:45:08" << std::endl;
	process("14:31:45 14:46:56");
	std::cout << "00:15:11" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string st) {
	std::istringstream ss(st);
	int h,m,s;
	char z;
	ss >> h >> z >> m >> z >> s;
	int t1 = h * 3600 + m * 60 + s;
	ss >> h >> z >> m >> z >> s;
	int t2 = h * 3600 + m * 60 + s;
	t1 -= t2;
	if( t1 < 0 ) {
		t1 = -t1;
	}
	std::cout << std::setfill('0') << std::setw(2)  <<  t1 / 3600 << ':' 
		<< std::setfill('0') << std::setw(2) << (t1 % 3600 ) / 60 << ':' 
		<< std::setfill('0') << std::setw(2) << t1 %60 << std::endl;
}
