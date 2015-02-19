/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
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
    process("Lorem ipsum dolor sit amet. Mea et habeo doming praesent. Te inani utroque recteque has, sea ne fugit verterem!");
    std::cout << "Lorem ipsum dolor sit amet. Mea et habeo doming praesent, yeah! Te inani utroque recteque has, sea ne fugit verterem!"  "\n";// std::endl for flush?
    process("Usu ei scripta phaedrum, an sed salutatus definiebas? Qui ut recteque gloriatur reformidans. Qui solum aeque sapientem cu.");
    std::cout << "Usu ei scripta phaedrum, an sed salutatus definiebas, this is crazy, I tell ya. Qui ut recteque gloriatur reformidans. Qui solum aeque sapientem cu, can U believe this?"  "\n";// std::endl for flush?
    process("Eu nam nusquam quaestio principes.");
    std::cout << "Eu nam nusquam quaestio principes."  "\n";// std::endl for flush?

}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

namespace {
    int n = 0;
    int z = 0;
    const std::vector<std::string> lst = {
        ", yeah!",
        ", this is crazy, I tell ya.",
        ", can U believe this?",
        ", eh?",
        ", aw yea.",
        ", yo.",
        "? No way!",
        ". Awesome!",
        };
    const std::string pc {".!?"};
}

void process(std::string s) {
//	std::cout << "s = '" << s << "'\n";
    for(auto c : s) {
        if(pc.find(c) != std::string::npos && (1&n++)) {
            std::cout << lst[(z++)%lst.size()];
        } else {
            std::cout << c;
        }
    }
    std::cout << "\n";
}
