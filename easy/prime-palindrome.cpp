/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
long rev(long v) {
	long r = 0;
	while(v) {
		r = r * 10 + v % 10;
		v /= 10;
	}
	return r;
}

int main(int _a, char ** _v) {
	int cnt = 2;
	long res = 11;
	long cur = 5;
	std::vector<int> primes{3};
	while(cur < 1000) {
		if(primes.end() == std::find_if(primes.begin(), primes.end(), [cur](int v){return 0==cur%v;})) {
			primes.push_back(cur);
			if(cur == rev(cur)) {
				res = cur;
			}
			++cnt;
		}
		cur += 2;
	}
	std::cout << res << std::endl;
}

