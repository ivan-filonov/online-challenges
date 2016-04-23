#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

void bubble_sort(vector<int> &v) 
{
    int n = v.size();
    bool flag = false;
    while (!flag)
    {
        flag = true;
        for (int i = 0; i < n - 1; i++)
            if (v[i] > v[i + 1])
                swap(v[i], v[i + 1]), flag = false;
    }
}

int main()
{
    freopen("game.in", "r", stdin);
    freopen("game.out", "w", stdout);
    int t;
    scanf("%d", &t);
    while (t--)
    {
        int n, l;
        scanf("%d%d", &n, &l);
        vector<int> a(n), b(n);
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        for (int i = 0; i < n; i++)
            scanf("%d", &b[i]);
        bubble_sort(a);
        bubble_sort(b);
        reverse(b.begin(), b.end());
        long long sum_a = 0;
        long long sum_b = 0;
        for (int i = 0; i < l; i++)
            sum_a += a[i], sum_b += b[i];
        if (sum_a > sum_b)
            printf("YES\n");
        else 
            printf("NO\n");
    }
    return 0;    
}