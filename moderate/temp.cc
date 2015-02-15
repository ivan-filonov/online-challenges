#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
std::vector<std::string> vec;
void adds(std::string s) {
	vec.push_back(s);
	std::sort(vec.begin(), vec.end(), [](const std::string& a, const std::string& b){
		return a.length() > b.length();
	});
	std::cout << "adds('" << s << "') -> ";
	for(auto it = vec.begin(); vec.end() != it; ++it) {
		if(vec.begin() != it) {
			std::cout << ',';
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}
int main() {
	adds("s1");
	adds("s12");
	adds("s1347");
	adds("s156");
	return 0;
}

