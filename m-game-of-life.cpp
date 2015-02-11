#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
int live_count(vector<string> &m, int v, int h) {
	int res = 0;
	for(int i = v-1; i <= v+1; ++i) {
		if(i < 0 || i >= m.size()) {
			continue;
		}
		const auto& line = m[i];
		for(int j = h-1; j <= h+1; ++j) {
			if((i==v) && (j==h)) {
				continue;
			}
			if(j < 0 || j >= line.length()) {
				continue;
			}
			if('*' == line[j]) {
				++res;
			}
		}
	}
	return res;
}
void process(vector<string> &m) {
	vector<string> cur(m), next(m);
	for(int step = 0; step < 10; ++step) {
		for(int v = 0; v < cur.size(); ++v) {
			for(int h = 0; h < cur[v].length(); ++h) {
				auto nc = live_count(cur, v, h);
				auto cell = cur[v][h];
				if(2==nc) {
					//ignore
				}
				else if(3==nc) {
					cell = '*';
				}
				else {
					cell = '.';
				}
				next[v][h] = cell;
			}
		}
		swap(cur, next);
	}
	for(auto s: cur) cout << s << endl;
}

void test(){
	vector<string> test_data {
		".........*",
		".*.*...*..",
		"..........",
		"..*.*....*",
		".*..*...*.",
		".........*",
		"..........",
		".....*..*.",
		".*....*...",
		".....**...",
	};	
	vector<string> expected {
		"..........",
		"...*......",
		"..*.*.....",
		"..*.*.....",
		"...*......",
		"..........",
		"..........",
		"..........",
		"..........",
		"..........",
	};

	cout << live_count(test_data, 0, 0) << " expect 1" << endl;
	cout << live_count(test_data, 1, 1) << " expect 0" << endl;
	cout << live_count(test_data, 0, 8) << " expect 2" << endl;
	cout << live_count(test_data, 2, 2) << " expect 3" << endl;
	cout << live_count(test_data, 1, 2) << " expect 2" << endl;
	
	process(test_data);
	cout << "expected: " << endl;
	for(auto s : expected) {
		cout << s << endl;
	}
}
int main(int _c,char**_v){
	//test();
	ifstream stream(_v[1]);
	string line;
	vector<string> field;
	while(getline(stream, line)) {
		field.push_back(line);
	}
	process(field);
	return 0;
}
