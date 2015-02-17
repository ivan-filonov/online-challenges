/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>

int main(int _a, char ** _v) {
	int a = 1;
	char b = reinterpret_cast<char*>(&a)[0];
	std::cout << ((!b)?"Big":"Little") << "Endian\n";
	return 0;
}

