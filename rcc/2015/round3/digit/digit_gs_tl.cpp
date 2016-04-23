#include<cstdio>
#include<algorithm>
#include<vector>
#include<iostream>
using namespace std;

int digit_root(long long n)
{
    return (n - 1) % 9 + 1;
}

vector<int> solve(long long a, long long b)
{
    vector<int> cnt(10, 0);
    for (long long i = a; i <= b; i++)
        cnt[digit_root(i)]++;
    vector<int> res(1, 0);
    for (int i = 1; i < 10; i++)
    {
        if (cnt[res[0]] < cnt[i])
            res = vector<int>(1, i);
        else if (cnt[res[0]] == cnt[i])
            res.push_back(i);
    }
    cerr << res.size() << endl;
    return res;
}

void print(vector<int> const &a)
{
    int n = a.size();
    cout << n << ' ';
    for (int i = 0; i < n; i++)
        cout << a[i] << ' ';
    cout << endl;
}

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        long long a, b;
        cin >> a >> b;
        print(solve(a, b));
    }
    return 0;
}