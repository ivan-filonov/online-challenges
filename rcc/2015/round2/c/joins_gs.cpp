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
        double ans = 0;
        long long sum = 0;
        for (int i = 0; i < n; i++)
        {
            if (a[i] > 2 * ans)
            {
                if (a[i] / 2.0 > sum)
                    ans = a[i] - sum;
                else
                    ans = a[i] / 2.0;
            }
            sum += a[i];
        }
        printf("%f\n", ans);
        //cerr << ans << ' '  << ans_sum << endl;
    }
    return 0;
}