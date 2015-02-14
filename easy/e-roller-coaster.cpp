#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
void process_line(string s) {
	bool flag = false;
	transform(s.cbegin(), s.cend(), s.begin(), [&flag](char c){
		if(isalpha(c)) {
			flag = !flag;
			return flag ? toupper(c):tolower(c);
		} else {
			return (int)c;
		}
	});
	cout << s << endl;
}
void test() {
	auto src={
		"To be, or not to be: that is the question.",
		"Whether 'tis nobler in the mind to suffer.",
		"The slings and arrows of outrageous fortune.",
		"Or to take arms against a sea of troubles.",
		"And by opposing end them, to die: to sleep."
	};
	auto exp={
		"To Be, Or NoT tO bE: tHaT iS tHe QuEsTiOn.",
		"WhEtHeR 'tIs NoBlEr In ThE mInD tO sUfFeR.",
		"ThE sLiNgS aNd ArRoWs Of OuTrAgEoUs FoRtUnE.",
		"Or To TaKe ArMs AgAiNsT a SeA oF tRoUbLeS.",
		"AnD bY oPpOsInG eNd ThEm, To DiE: tO sLeEp."
	};
	for(auto si=src.begin(), di=exp.begin(); src.end() != si; ++si, ++di) {
		process_line(*si);
		cout  << *di << endl;
	}
}
int main(int argc, char ** argv) {
//	test();
	ifstream stream(argv[1]);
	string line;
	while(std::getline(stream, line)) {
		process_line(line);
	}
	return 0;
}
