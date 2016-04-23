#include<bits/stdc++.h>
using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++)
            cin >> a[i];
        long long ans = a[0];
        long long ans_sum = 0;
        long long sum = 0;
        for (int i = 0; i < n; i++)
        {
            if (a[i] > 2 * ans)
                ans = a[i], ans_sum = sum;
            sum += a[i];
        }
        //cerr << ans << ' '  << ans_sum << endl;
        if (ans > 2 * ans_sum)
            printf("%.6f\n", 1.0 * ans - ans_sum);
        else
            printf("%.6f\n", ans / 2.0);
    }
    return 0;
}