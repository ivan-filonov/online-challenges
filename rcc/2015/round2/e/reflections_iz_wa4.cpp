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

const int maxn = 100100;

int gcd(int a, int b) {
	while (b) b ^= a ^= b ^= a %= b;
	return abs(a);
}

struct pt {
	int x, y;

	pt() {}
	pt(int x, int y) : x(x), y(y) {}

	void read() {
		scanf("%d%d", &x, &y);
	}

	pt operator+ (const pt &p) {
		return pt(x + p.x, y + p.y);
	}

	pt operator- (const pt &p) {
		return pt(x - p.x, y - p.y);
	}

	pt operator* (const int &p) {
		return pt(x * p, y * p);
	}

	bool operator== (const pt &p) {
		return x == p.x && y == p.y;
	}

	int operator^ (const pt &p) {
		return 1LL * x * p.y - 1LL * p.x * y;
	}

	int operator* (const pt &p) {
		int g = gcd(y, p.y);
		if (g == 0) return gcd(x, p.x);
		return abs(1LL * x * p.y / g - 1LL * p.x * y / g);
	}

	pt norm() {
		pt res = pt(x, y);
		if (y < 0 || y == 0 && x < 0) {
			res.x *= -1;
			res.y *= -1;
		}
		return res;
	}
};

pt gcd(pt a, pt b) {
	if (a.y < b.y) swap(a, b);
	while (!(b == pt(0, 0)) && !(a == pt(0, 0))) {
		a = a - b;
		if (a.y < b.y) swap(a, b);
	}
	return a;
}

int n;
pt a[maxn], s, f;

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

bool find_euclid(int a, int b, int c, int &x0, int &y0, int &g) {
	g = gcd(abs(a), abs(b), x0, y0);
	if (c % g) return false;
	x0 *= c / g;
	y0 *= c / g;
	if (a < 0) x0 *= -1;
	if (b < 0) y0 *= -1;
	return true;
}

pt find_sol(pt a, pt b, pt c) {
	int need = gcd(a.y, gcd(b.y, c.y));
	// need - ai * a.y = 0 (% gby)
	// n1 * a.y = need (% gby)
	int k, g;
	int ai;
	assert(find_euclid(a.y, gcd(b.y, c.y), need, ai, k, g));
	int bi, ci;
	assert(find_euclid(b.y, c.y, need - ai * a.y, bi, ci, g));
	return (a * ai + b * bi + c * ci);
}

bool check(pt a, pt b, pt c) {
	if (b == pt(0, 0)) {
		return (a ^ c) == 0 && gcd(a, c.norm()) == a;
	}
	int det = a.x * b.y - b.x * a.y;
	int detx= c.x * b.y - b.x * c.y;
	int dety= a.x * c.y - c.x * a.y;
	return detx % det == 0 && dety % det == 0;
}

int some_function(pt v1, pt v2, pt v) {
    // find k that k * v.y = 0 (% v2.y)
    if (v2 == pt(0, 0)) {
        return v1 * v;
    }
    int x0, y0, g;
    assert(find_euclid(v.y, v2.y, 0, x0, y0, g));
    int l = x0 / (v2.y / g);
    x0 -= l * v2.y / g;
    y0 -= l * v.y / g;
    if (x0 < 0) {
        x0 += v2.y / g;
        y0 += v.y / g;
    }
    return gcd(v1.x, x0);
}

string solve() {
	if (n == 1) {
		return ((s == f || a[0] * 2 - s == f) ? "YES" : "NO");
	}
	vector<pt> v;
	for (int i = 1; i < n; i++) {
		v.push_back((a[i] - a[0]) * 2);
		if (v.back().y < 0 || v.back().y == 0 && v.back().x < 0) v.back() = v.back().norm();
	}
	
	pt v1 = v[0], v2(0, 0);
	for (int i = 1; i < (int)v.size(); i++) {
		if (v2 == pt(0, 0) && (v1 ^ v[i]) == 0) {
			v1 = gcd(v1, v[i]);
			continue;
		}
		pt b1 = pt(some_function(v1, v2, v[i]), 0);
		pt b2 = find_sol(v1, v2, v[i]);
		while (b2.x >= b1.x) b2 = b2 - b1;
		v1 = b1;
		v2 = b2;
	}
	
	bool res = 0;
	res |= check(v1, v2, f - s);
	for (int i = 0; i < n; i++) res |= check(v1, v2, f - (a[i] * 2 - s));
	return (res ? "YES" : "NO");
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
