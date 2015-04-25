#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

const int MOD = 1e9 + 7;

int a[1111][1111][2];
double w[1111][1111];
double w_e[1111][1111][2];
double p[1111][1111];
double p_e[1111][1111][2];

void solve() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++)
            for (int k = 0; k < 2; k++)
                scanf("%d", &a[i][j][k]);

    p[0][0] = 1;
    w[0][0] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            if (a[i][j][0] == a[i][j][1]) {
                p_e[i][j][0] = p_e[i][j][1] = p[i][j] / 2;
                w_e[i][j][0] = w_e[i][j][1] = w[i][j] + a[i][j][0];
            } else {
                if (a[i][j][0] < a[i][j][1]) {
                    p_e[i][j][0] = p[i][j];
                    w_e[i][j][0] = w[i][j] + a[i][j][0];
                    p_e[i][j][1] = w_e[i][j][1] = 0;
                } else {
                    p_e[i][j][1] = p[i][j];
                    w_e[i][j][1] = w[i][j] + a[i][j][1];
                    p_e[i][j][0] = w_e[i][j][0] = 0;
                }
            }

        }
        for (int j = 0; j <= i + 1; j++) {
            double p1 = 0, p2 = 0;
            double w1 = 0, w2 = 0;
            p[i + 1][j] = w[i + 1][j] = 0;
            if (j != 0) {
                p[i + 1][j] += p_e[i][j - 1][1]; 
                p1 = p_e[i][j - 1][1];
                w1 = w_e[i][j - 1][1];
            }
            if (j != i + 1) { 
                p[i + 1][j] += p_e[i][j][0];
                p2 = p_e[i][j][0];
                w2 = w_e[i][j][0];
            }
            if ((p1 + p2) > 1e-9) {
                w[i + 1][j] = (w1 * p1 + w2 * p2) / (p1 + p2);
            }
        }
    }
    double res = 0;
    for (int j = 0; j <= n; j++) {
        res += p[n][j] * w[n][j];
    }
    printf("%.19f\n", res);
    //cout << res << "\n";
}

int main() {
	int t;
	scanf("%d", &t);
//	cin >> t;
	while (t--) solve();
}
