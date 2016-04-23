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
    vector<int> res;
    a = digit_root(a);
    b = digit_root(b);
    if (a <= b)
    {
        for (int i = a; i <= b; i++)
            res.push_back(i);
    }
    else
    {
        for (int i = a; i <= 9; i++)
            res.push_back(i);
        for (int i = 1; i <= b; i++)
            res.push_back(i);
        //sort(res.begin(), res.end());
    }
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