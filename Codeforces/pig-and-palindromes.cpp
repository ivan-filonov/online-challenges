#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

const int modulo = 1000 * 1000 * 1000 + 7;
int rows, cols;
std::vector<char> m;

void readall() {
	std::cin >> rows >> cols;
	m.resize(rows * cols);

	std::string t;
	for(int i = 0; i < rows; ++i) {
		std::cin >> t;
		std::copy(t.begin(), t.end(), m.begin() + i * cols);
	}
}

std::map<unsigned int, unsigned int> mem;

unsigned int paths(unsigned int dr, unsigned int dc, unsigned int os, unsigned int od) {
	if(m[os] != m[od]) {
		return 0;
	}

	if(dc + dr > 8) {
		if(mem.count(os * 65536 + od)) {
			return mem[os * 65536 + od];
		}
	}

	if(!dr || !dc) {
		unsigned int incr = dr ? cols : 1;
		while(os < od) {
			if(m[os] != m[od]) {
				return 0;
			}
			os += incr;
			od -= incr;
		}
		return 1;
	}

	if(1 == dr && 1 == dc) {
		return 2;
	}

	unsigned int r = 
		paths(dr - 1, dc - 1, os + cols, od - 1) +
		paths(dr - 1, dc - 1, os + 1, od - cols);
	if(dr > 1) {
		r += paths(dr - 2, dc, os + cols, od - cols);
	}
	if(dc > 1) {
		r += paths(dr, dc - 2, os + 1, od - 1);
	}

	r %= modulo;
	if(dc + dr > 8) {
		mem[os * 65536 + od] = r;
	}
	return r;
}

#include <chrono>
int main() {
	std::cin.sync_with_stdio(false);

//	readall();
	cols = rows = 500;
	m = std::vector<char>(500 * 500, 'a');

	auto t = std::chrono::steady_clock::now();
	std::cout << paths(rows - 1, cols - 1, 0, rows * cols - 1);
	std::cout << std::endl << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t).count() << "\n";
	
	return 0;
}
