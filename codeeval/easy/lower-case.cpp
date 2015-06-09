/* Sample code to read in test cases: */
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
 using namespace std;
 int main(int argc, char *argv[]) {
 	ifstream stream(argv[1]);
	string line;
	while (getline(stream, line)) {
		// Do something with the line
		std::transform(line.begin(), line.end(), line.begin(), [](char c){return tolower(c);});
		cout << line << endl;
	}
	return 0;
}
