/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
    process("abcdefghik");
    std::cout << "012345678"  "\n";
    process("Xa,}A#5N}{xOBwYBHIlH,#W");
    std::cout << "05"  "\n";
    process("(ABW>'yy^'M{X-K}q,");
    std::cout << "NONE"  "\n";
    process("6240488");
    std::cout << "6240488"  "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
    int cnt = 0;
    for(auto c : s) {
        if((c >= '0' && c <= '9') || (c >= 'a' && c <= ('a' + 9))) {
            ++cnt;
            if(!std::isdigit(c)) {
                c -= 'a' - '0';
            }
            std::cout << c;
        }
    }
    if(!cnt) {
        std::cout << "NONE";
    }
    std::cout << "\n";
}
