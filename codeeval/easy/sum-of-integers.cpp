#include <fstream>
#include <iostream>
#include <string>
using namespace std;
int main(int argc, char**argv) {
	std::ifstream stream(argv[1]);
	string line;
	long long sum = 0;
	while(std::getline(stream, line)) {
		sum += std::stoll(line);
	}
	cout << sum << endl;
	return 0;
}
