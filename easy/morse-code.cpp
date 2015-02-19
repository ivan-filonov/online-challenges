/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
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
	process(".- ...- ..--- .-- .... .. . -.-. -..-  ....- .....");
	std::cout << "AV2WHIECX 45"  "\n";// std::endl for flush?
	process("-... .... ...--");
	std::cout << "BH3"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

namespace {
    std::map<std::string,std::string> mm = {{ ".----", "1" },{ "--.-", "Q" },{ ".....", "5" },{ "--.", "G" },{ ".--.", "P" },{ "-..-", "X" },{ "-.--", "Y" },{ ".--", "W" },{ "-.-.", "C" },{ "-..", "D" },{ "-.", "N" },{ "....-", "4" },{ "-.-", "K" },{ ".-..", "L" },{ "--..", "Z" },{ ".-", "A" },{ "---", "O" },{ "--...", "7" },{ ".-.", "R" },{ "-....", "6" },{ "...--", "3" },{ "..-", "U" },{ "-", "T" },{ "...", "S" },{ "-...", "B" },{ "-----", "0" },{ "...-", "V" },{ "--", "M" },{ "----.", "9" },{ "....", "H" },{ "..", "I" },{ "---..", "8" },{ ".---", "J" },{ "..---", "2" },{ ".", "E" },{ "..-.", "F" }};
}

void process(std::string s) {
    std::string t;
    t.reserve(10);
    for(char c : s) {
        if(' ' == c) {
            if(t.empty()) {
                std::cout << ' ';
            } else {
                std::cout << mm[t];
                t.clear();
            }
        } else {
            t.append(1, c);
        }
    }
    if( !t.empty() ) {
        std::cout << mm[t];
    }
    std::cout << "\n";
}
