/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("0123456789");
	std::cout << ""  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

namespace {
    std::vector<std::string> cm {
        "-**----*--***--***---*---****--**--****--**---**--",
        "*--*--**-----*----*-*--*-*----*-------*-*--*-*--*-",
        "*--*---*---**---**--****-***--***----*---**---***-",
        "*--*---*--*-------*----*----*-*--*--*---*--*----*-",
        "-**---***-****-***-----*-***---**---*----**---**--",
        "--------------------------------------------------"
    };
}

void process(std::string s) {
    std::vector<std::ostringstream> v(6);
    for(auto c : s) {
        if(!std::isdigit(c)) {
            continue;
        }
        c -= '0';
        c *= 5;
        for(int i = 0; i < v.size(); ++i) {
            std::copy(std::begin(cm[i]) + c, std::begin(cm[i]) + c + 5, std::ostream_iterator<char>(v[i]));
        }
    }
    for(auto& os : v) {
        std::cout << os.str() << "\n";
    }
}
