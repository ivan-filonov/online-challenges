#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

int a[1111][1111][2];
double w[1111][1111];

void solve() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++)
            for (int k = 0; k < 2; k++)
                scanf("%d", &a[i][j][k]);


	for (int i = 0; i <= n; i++)
		w[n][i] = 0;

    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            if (a[i][j][0] == a[i][j][1]) {
                w[i][j] = (w[i + 1][j] + w[i + 1][j + 1]) / 2 + a[i][j][0];
            } else {
                if (a[i][j][0] < a[i][j][1]) {
                    w[i][j] = w[i + 1][j] + a[i][j][0];
                } else {
                    w[i][j] = w[i + 1][j + 1] + a[i][j][1];
                }
            }
        }
    }    
    printf("%.19f\n", w[0][0]);
    //cout << w[0][0] << "\n";
}

int main() {
	int t;
	scanf("%d", &t);
	//cin >> t;
	while (t--) solve();
}

