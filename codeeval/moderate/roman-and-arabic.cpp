/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
int conv(char c) {
    switch(c) {
        case 'I' : return 1;
        case 'V' : return 5;
        case 'X' : return 10;
        case 'L' : return 50;
        case 'C' : return 100;
        case 'D' : return 500;
        case 'M' : return 1000;
    }
    throw c;
}

void test();
void process_file(char*);
int main(int argc, char ** argv) {
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("3M1D2C");
	std::cout << "3700"  "\n";// std::endl for flush?
	process("2I3I2X9V1X");
	std::cout << "-16"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
    std::vector<int> v;
    v.reserve(s.length());
    for(int i = 0; i < s.length(); i+=2) {
        v.push_back(s[i] - '0');
        v.push_back(conv(s[i+1]));
        if(i > 0 && std::abs(v[i + 1]) > std::abs(v[i - 1])) {
            v[i - 1] = -v[i - 1];
        }
    }
    int acc = 0;
    for(int i = 0; i < v.size(); i += 2) {
        acc += v[i] * v[i+1];
    }
    std::cout << acc << "\n";
}
