#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long ll;

const int MAXN = 1 << 11;

struct pt {
    int x, y;

    pt() {}
    pt(int x, int y) : x(x), y(y) {}

    void read() {
        scanf("%d%d", &x, &y);
    }
};

int n;
pt a[MAXN];

bool reading() {
    if (!(scanf("%d", &n) == 1)) return 0;
    for (int i = 0; i < n; i++) a[i].read();
    return 1;
}

ll gcd(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

int o[MAXN * MAXN + 10];
int fastsolve() {
    for (int i = 1; i < MAXN; i++) o[i * (i - 1) / 2] = i;
    vector<pair<pair<int, int>, pair<ll, ll> > > vct;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            ll A = a[i].y - a[j].y;
            ll B = a[j].x - a[i].x;
            ll C = -(A * a[i].x + B * a[i].y);
            ll g = gcd(abs(A), abs(B));
            A /= g;
            B /= g;
            if (A < 0 || A == 0 && B < 0) {
                A *= -1;
                B *= -1;
                C *= -1;
            }
            ll gg = gcd(g, abs(C));
            C /= gg;
            g /= gg;

            vct.push_back(make_pair(make_pair(A, B), make_pair(C, g)));
        }
    }
    sort(vct.begin(), vct.end());
    int ans = 1;
    for (int L = 0; L < (int)vct.size(); L++) {
        int R = L;
        vector<int> v;
        while (R < (int)vct.size() && vct[R].first == vct[L].first) {
            int cnt = 0;
            while (R + cnt < (int)vct.size() && vct[R + cnt] == vct[R]) cnt++;
            v.push_back(o[cnt]);
            R += cnt;
        }
        if (v.size() == 1) {
            ans = max(ans, v[0] + (v[0] != n));
        } else {
            nth_element(v.begin(), v.end() - 2, v.end());
            ans = max(ans, v[(int)v.size() - 1] + v[(int)v.size() - 2]);
        }
        L = R - 1;
    }
    return ans;
}

int main() {
    //freopen("points.in", "r", stdin);
    //freopen("points.out", "w", stdout);

    int T;
    scanf("%d", &T);
    while (T--) {
        reading();
        printf("%d\n", fastsolve());
    }

    return 0;
}
