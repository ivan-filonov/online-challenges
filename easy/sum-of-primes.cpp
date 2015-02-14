/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
int main(int _a, char ** _v) {
	long sum = 5;
	int cnt = 2;
	long cur = 5;
	std::vector<int> primes{3};
	while(cnt < 1000) {
		if(primes.end() == std::find_if(primes.begin(), primes.end(), [cur](int v){return 0==cur%v;})) {
			primes.push_back(cur);
			sum += cur;
			++cnt;
		}
		cur += 2;
	}
	std::cout << sum << std::endl;
}

