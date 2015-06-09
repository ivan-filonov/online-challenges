/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
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
    process("Feb 2004-Dec 2009; Sep 2004-Jul 2008");
    std::cout << "5\n";
    process("Aug 2013-Mar 2014; Apr 2013-Aug 2013; Jun 2014-Aug 2015; Apr 2003-Nov 2004; Apr 2014-Jan 2015");
    std::cout << "4\n";
    process("Mar 2003-Jul 2003; Nov 2003-Jan 2004; Apr 1999-Nov 1999");
    std::cout << "1\n";
    process("Apr 1992-Dec 1993; Feb 1996-Sep 1997; Jan 2002-Jun 2002; Sep 2003-Apr 2004; Feb 2010-Nov 2011");
    std::cout << "6\n";
    process("Feb 2004-May 2004; Jun 2004-Jul 2004");
    std::cout << "0\n";

}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}
namespace {
    const std::string months[] { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
}
void process(std::string s) {
    std::istringstream ss { s };
    std::string m1, y1, m2, y2;
    std::set<int> ms;
    
    using std::getline;
//    std:: cout << s << "\n";
    while(getline(ss, m1, ' ') && getline(ss, y1, '-') && getline(ss, m2, ' ') && getline(ss, y2, ';')) {
//        std::cout << m1 << " -> " << m1 << ", " << m2 << " -> " << m2 << "\n";
        auto mi1 = std::find(std::begin(months), std::end(months), m1) - std::begin(months);
        auto mi2 = std::find(std::begin(months), std::end(months), m2) - std::begin(months);
//        std::cout << m1 << " -> " << mi1 << ", " << m2 << " -> " << mi2 << "\n";
        ss.ignore(10, ' ');
        int d1 = mi1 + std::stoi(y1) * 12;
        int d2 = mi2 + std::stoi(y2) * 12;
        for(auto i = d1; i <= d2; ++i) {
            ms.insert(i);
        }
    }
    std::cout << ms.size()/12 << "\n";
}
