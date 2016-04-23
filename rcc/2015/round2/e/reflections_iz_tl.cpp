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

	ll operator^ (const pt &p) {
		return 1LL * x * p.y - 1LL * p.x * y;
	}

	ll operator* (const pt &p) {
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

// todo
pt find_sol(pt a, pt b, pt c) {
	int need = gcd(a.y, gcd(b.y, c.y));
	for (int mx = 0;; mx++) {
		for (int i = -mx; i <= mx; i++) {
			for (int j = -mx; j <= mx; j++) {
				int d = 1;
				if (abs(i) != mx && abs(j) != mx) {
					d = 2 * mx;
				}
				for (int k = -mx; k <= mx; k += d) {
					if ((a * i + b * j + c * k).y == need) {
						return (a * i + b * j + c * k);
					}
				}
			}
		}
	}
}

bool check(pt a, pt b, pt c) {
	if (b == pt(0, 0)) {
		return (a ^ c) == 0 && gcd(a, c.norm()) == a;
	}
	ll det = a.x * b.y - b.x * a.y;
	ll detx= c.x * b.y - b.x * c.y;
	ll dety= a.x * c.y - c.x * a.y;
	return detx % det == 0 && dety % det == 0;
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
		pt b1 = pt(gcd(v1 * v2, gcd(v1 * v[i], v2 * v[i])), 0);
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
