#include <iostream>
#include <sstream>
int main() {
	int n;
	std::cin.sync_with_stdio(false);
	std::cin >> n;
	for(int i = 0; i !=n; ++i) {
		std::string word;
		std::cin >> word;
		std::ostringstream ss;
		if(word.length() > 10) {
			ss << word.front() << (word.length() - 2) << word.back();
		} else {
			ss << word;
		}
		std::cout << ss.str() << "\n";
	}
	return 0;
}
