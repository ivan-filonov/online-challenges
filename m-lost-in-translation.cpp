#include <fstream>
#include <iostream>
#include <map>
#include <string>
const char * c_from = "abcdefhgijklmnopqrstuvwxyz";
const char * c_to   = "yhesocxvduiglbkrztnwjpfmaq";
std::map<char,char> tab;
void process(std::string line) {
	std::string res;
	res.reserve(line.length());
	for(auto c : line) {
		auto iter = tab.find(c);
		if(tab.end() == iter) {
			res += c;
		} else {
			res += iter->second;
		}
	}
	std::cout << res << std::endl;
}
int main(int _c, char** _v) {
	for(int i = 0; c_from[i]; ++i) {
		tab[c_from[i]] = c_to[i];
	}
//	process("rbc vjnmkf kd yxyqci na rbc zjkfoscdd ew rbc ujllmcp");
//	process("tc rbkso rbyr ejp mysljylc kd kxveddknmc re jsicpdrysi");
//	process("de kr kd eoya kw aej icfkici re zjkr");
	std::ifstream stream(_v[1]);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

