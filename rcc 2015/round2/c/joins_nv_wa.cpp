#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <cmath>
#include <cstdio>

using namespace std;

int main() {
    
    int t;
    scanf("%d", &t);
    vector<int> a;
    while (t--) {
        int n;
        scanf("%d", &n);
        a.resize(n);
        int pos = 0;
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
            if (a[pos] < a[i])
                pos = i;
        }
        long long sum = 0;
        double ans = a[0];
        for (int i = 0; i < n; i++) {
            //cerr << a[i] << " " << sum <<  " " << a[pos] << "\n";
            if (pos == i) {
                ans = max(ans, (a[pos] <= 2 * sum) ? a[pos] * 0.5 : a[pos] - sum);
                break;
            }
            sum += a[i];
        }
//        cerr << ans << endl;
        printf("%.15f\n", ans);
    }

}

