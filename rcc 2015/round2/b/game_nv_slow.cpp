#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

const int MOD = 1e9 + 7;

int n, cnt;
long long sum;

int a[1000][1000][2];

void dfs(int i, int j, long long c = 1, long long cur = 0) {
    if (i == n) {
        cnt += c;
        sum += cur * c;
        return;
    }
    if (a[i][j][0] == a[i][j][1]) {
       dfs(i + 1, j, c, cur + a[i][j][0]);
       dfs(i + 1, j + 1, c, cur + a[i][j][1]); 
       return;
    }
    if (a[i][j][0] < a[i][j][1]) {
        dfs(i + 1, j, 2 * c, cur + a[i][j][0]);
        return;
    }
    dfs(i + 1, j + 1, 2 * c, cur + a[i][j][1]);    
}
void solve() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++)
            for (int k = 0; k < 2; k++)
                scanf("%d", &a[i][j][k]);

	sum = cnt = 0;
    dfs(0, 0);    
    printf("%.19f\n", ((sum * 1.) / cnt));
}

int main() {
	int t;
//	cin >> t;
    scanf("%d", &t);
	while (t--) solve();
}

