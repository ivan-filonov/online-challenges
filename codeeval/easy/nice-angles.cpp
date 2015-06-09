/*
 * Common base C++ source for codeeval solutions.
 * */
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	std::cout << std::setfill('0') << std::setw(2);
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("330.39991833");
	std::cout << "330.23'59\"" << std::endl;
	process("0.001");
	std::cout << "0.00'03\"" << std::endl;
	process("14.64530319");
	std::cout << "14.38'43\"" << std::endl;
	process("0.25");
	std::cout << "0.15'00\"" << std::endl;
	process("254.16991217");
	std::cout << "254.10'11\"" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	auto v = std::stod(s);
	int angle = v * 3600.0;
	std::cout << angle/3600 << '.'  
		<< std::setfill('0') << std::setw(2)<< (angle/60)%60 << '\''  
		<< std::setfill('0') << std::setw(2)<< angle%60 << '"' 
		<<  std::endl;
}
