#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <set>
#include <queue>
#include <sstream>
#include <deque>
#include <memory.h>
#include <cassert>
#include <ctime>
#include <cstring>


using namespace std;

#define ll long long
#pragma comment(linker, "/STACK:64000000")

const int maxn = 200200;
const int inf = 1000000007;
const int mod = 1000000007;
const int dx[4] = {1, -1, 0, 0};
const int dy[4] = {0, 0, 1, -1};

struct pt {
    ll x, y;

    pt() {}
    pt(ll x, ll y) : x(x), y(y) {}

    void read() {
        int _x, _y;
        scanf("%d%d", &_x, &_y);
        x = _x;
        y = _y;
    }

    void print() {
        cout << x << " " << y << endl;
    }

    pt operator+ (const pt &p) {
        return pt(x + p.x, y + p.y);
    }

    pt operator- (const pt &p) {
        return pt(x - p.x, y - p.y);
    }

    pt operator* (const ll &p) {
        return pt(x * p, y * p);
    }

    ll operator^ (const pt &p) {
        return x * p.y - p.x * y;
    }

    bool operator== (const pt &p) {
        return x == p.x && y == p.y;
    }

    pt norm() {
        if (y > 0 || y >= 0 && x >= 0) return pt(x, y);
        return pt(-x, -y);
    }
};

int n;
pt a[maxn];
pt s, f;

bool reading() {
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; i++) a[i].read();
    s.read();
    f.read();
    return 1;
}

void print(string s) {
    printf("%s\n", s.c_str());
}

int sign(ll x) {
    if (x > 0) return 1;
    if (x == 0) return 0;
    return -1;
}

ll gcd(ll a, ll b) {
    a = abs(a);
    b = abs(b);
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// gcd(a, b), a || b
pt gcd(pt a, pt b) {
    if (a.y == 0 && b.y == 0) {
        return pt(gcd(a.x, b.x), 0);
    }
    int ny = gcd(a.y, b.y);
    int nx = gcd(a.x, b.x) * sign(a.x);
    return pt(nx, ny);
}

int gcd(int a, int b, int &x, int &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    int x1, y1;
    int d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

void euclid(int a, int b, int c, int &x, int &y, int &g) {
    g = gcd(abs(a), abs(b), x, y);
    assert(c % g == 0);
    x *= c / g;
    y *= c / g;
    if (a < 0) x = -x;
    if (b < 0) y = -y;
}

// find min (x, 0) point
pt f2(pt a, pt b, pt c) {
    // find k: k * c.y = 0 (% a.y)
    int g = gcd(a.y, c.y);
    ll nx = a.x * c.y / g - c.x * a.y / g;
    return pt(gcd(b.x, nx), 0);
}

// find min (_, y) point and norm it by d
pt f1(pt a, pt b, pt c, pt d) {
    // k1 * a.y + k2 * b.y + k3 * c.y == need, b.y = 0 -> k2 = 0
    int need = gcd(a.y, c.y);
    int k1, k2 = 0, k3, g;
    euclid(a.y, c.y, need, k1, k3, g);
    pt res = (a * k1 + b * k2 + c * k3);
    res = res - d * (res.x / d.x);
    if (res.x < 0) res = res + d;
    return res;
}

bool check(pt a, pt b, pt c) {
    if (b == pt(0, 0)) {
        bool res = (a ^ c) == 0;
        pt o = gcd(a, c);
        res &= o == a || o * -1 == a;
        return res;
    }
    ll det = 1LL * a.x * b.y - 1LL * b.x * a.y;
    ll detx= 1LL * c.x * b.y - 1LL * b.x * c.y;
    ll dety= 1LL * a.x * c.y - 1LL * c.x * a.y;
    return detx % det == 0 && dety % det == 0;
}

string solve() {
    if (n == 1) {
        return s == f || a[0] * 2 - s == f ? "YES" : "NO";
    }
    vector<pt> v;
    for (int i = 1; i < n; i++) v.push_back(((a[i] - a[0]) * 2).norm());
    pt v1 = v[0], v2 = pt(0, 0);
    // v2 is parallel to OX
    for (int i = 1; i < (int)v.size(); i++) {
        if (v2 == pt(0, 0) && (v1 ^ v[i]) == 0) {
            v1 = gcd(v1, v[i]);
        }
        pt b2 = f2(v1, v2, v[i]);
        pt b1 = f1(v1, v2, v[i], b2);
        v1 = b1;
        v2 = b2;
    }
    bool ans = false;
    ans |= check(v1, v2, f - s);
    for (int i = 0; i < n; i++) ans |= check(v1, v2, f - (a[i] * 2 - s));
    return ans ? "YES" : "NO";
}

int main() {
    //freopen("reflections.in", "r", stdin);
    //freopen("reflections.out", "w", stdout);
    //ios_base::sync_with_stdio(0);

    int T;
    scanf("%d", &T);
    while (T--) {
        reading();
        print(solve());
    }

    return 0;
}

