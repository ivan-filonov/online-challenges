#include <bits/stdc++.h>

using namespace std;

int main() {
    int T;
    scanf("%d", &T);
    int mx = 1e8 / T;
    while (T--) {
        int n;
        scanf("%d", &n);
        vector<int> x(n);
        vector<int> y(n);
        for (int i = 0; i < n; i++) scanf("%d%d", &x[i], &y[i]);
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);

        bool ok = 0;
        for (int it = 0; it < mx; it++) {
            if (x1 == x2 && y1 == y2) {
                ok = 1;
                break;
            }
            int i = rand() % n;
            x1 = 2 * x[i] - x1;
            y1 = 2 * y[i] - y1;
        }
        printf("%s\n", (ok ? "YES" : "NO"));
    }
}
