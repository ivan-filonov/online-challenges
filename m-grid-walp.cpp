#include <algorithm>
#include <iostream>
#include <stack>
#include <tuple>
#include <vector>
using namespace std;

int ds_calc(int n) {
	int s = 0;
	n = (n<0) ? -n : n;
	while(n>0) {
		s += n%10;
		n /= 10;
	}
	return s;
}

vector<int> dsums(600, 0);
void ds_prepare() {
	for(int i = 0; i < 600; ++i) {
		dsums[i] = ds_calc(i);
	}
}

int ds(int i) {
	return dsums[(i<0)?-i:i];
}

bool test(int x, int y) {
	return ds(x) + ds(y) <= 19;
}
int idx(int x,int y) {
	return (x+300) * 600 + y + 300;
}

int main() {
	ds_prepare();

	vector<char> points(600*600, 0);
	stack<int> sx, sy;
	
	sx.push(0);
	sy.push(0);

	while(!sx.empty()) {
		int x, y;
		tie(x,y) = make_tuple(sx.top(), sy.top());
		sx.pop(); sy.pop();
		int i = idx(x,y);
		if(points[i] || !test(x,y)) {
			continue;
		}
		points[i] = 1;
		sx.push(x+1-0);	sy.push(y+0-0);
		sx.push(x+0-0);	sy.push(y+1-0);
		sx.push(x+0-1);	sy.push(y+0-0);
		sx.push(x+0-0);	sy.push(y+0-1);
	}

	int sum = 0;
	std::for_each(points.begin(), points.end(), [&sum](char c){ sum += c; });

	cout << sum << endl;
}

