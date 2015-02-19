/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
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
    process("Rkbs,5453; Wdqiz,1245; Rwds,3890; Ujma,5589; Tbzmo,1303;");
    std::cout << "1245,58,2587,1563,136"  "\n";// std::endl for flush?

    process("Vgdfz,70; Mgknxpi,3958; Nsptghk,2626; Wuzp,2559; Jcdwi,3761;");
    std::cout << "70,2489,67,1135,197"  "\n";// std::endl for flush?

    process("Yvnzjwk,5363; Pkabj,5999; Xznvb,3584; Jfksvx,1240; Inwm,5720;");
    std::cout << "1240,2344,1779,357,279"  "\n";// std::endl for flush?

    process("Ramytdb,2683; Voclqmb,5236;");
    std::cout << "2683,2553"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
//	std::cout << "s = '" << s << "'\n";
    std::istringstream ss { s };
    std::string t, d;
    std::vector<int> v;
    while(std::getline(ss, t, ',') && std::getline(ss, d, ';')) {
        v.push_back(std::stoi(d));
    }
    std::sort(std::begin(v), std::end(v));
    int prev = 0;
    for(auto b = std::begin(v), it = b, e = std::end(v); e != it; ++it) {
        if(b != it) {
            std::cout << ',';
        }
        std::cout << (*it - prev);
        prev = *it;
    }
    std::cout << "\n";
}
