/*
 * Common base C++ source for codeeval solutions.
 * */
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
	process_file(_v[1]);
	return 0;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	double dist = std::numeric_limits<double>::max();
	std::vector<double> vx;
	std::vector<double> vy;
	int counter = 0;
	while(std::getline(stream, line)) {
		if( !counter ) {
			if( dist != std::numeric_limits<double>::max()) {
				if( dist < 10000 ) {
					printf("%.4f\n", dist);
				} else {
					std::cout << "INFINITY" << std::endl;
				}
				dist = std::numeric_limits<double>::max();
				vx.clear();
				vy.clear();
			}
			counter = std::stoi(line);
			if(!counter) {
				break;
			}
		} else {
			--counter;
			std::istringstream ss(line);
			double x,y;
			ss >> x >> y;
			for(auto ix = std::begin(vx), iy = std::begin(vy); 
				std::end(vx) != ix; 
				++ix, ++iy) {
				dist = std::min( dist, std::sqrt((x-*ix)*(x-*ix)+(y-*iy)*(y-*iy)) );
			}
			vx.push_back(x);
			vy.push_back(y);
		}
	}
}
