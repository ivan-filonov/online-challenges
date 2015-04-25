#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <cmath>

using namespace std;

vector<int> d;
vector<int> phi;
vector<int> pref;
vector<int> len;
int mod = 1000000023;//100000000;

void precalc() {
    int x = mod;
    d.resize(10);
    phi.resize(10);
    int l = 0;
    for (int i = 2; i * i <= x; i++) {
        if (x % i != 0)
            continue;

        d[l] = 1;
        while (x % i == 0) {
            d[l] *= i;
            x /= i;
        }
        phi[l] = d[l] / i * (i - 1);
        l++;
    }

    if (x > 1) {
        d[l] = x;
        phi[l++] = x - 1;
    }

    d.resize(l); // = Arrays.copyOf(d, l);
    phi.resize(l); // = Arrays.copyOf(phi, l);

    len.resize(l); // = new int[l];
    pref.resize(l); // = new int[l];

    for (int i = 0; i < l; i++) {
        int f1 = 1;
        int f2 = 1;
        //HashMap<Long, Integer> hm = new HashMap<Long, Integer>();
        map<long long, int> hm;
        int j = 0;
        while (true) {
            long long pairs = 1L * f1 * d[i] + f2;
            if (hm.count(pairs)) {
                len[i] = j - hm[pairs];
                pref[i] = hm[pairs];
                break;
            }
            hm[pairs] = j;

            int f = (f1 + f2) % d[i];
            f2 = f1;
            f1 = f;
            j++;
        }
    }
}

int pow(int x, long long p, int mod) {
    
    int ans = 1;
    x = x % mod;
    if (x == 0) return 0;
    while (p > 0) {
        if ((p & 1) == 1) {
            ans = (ans * x) % mod;
        }
        x = (x * x) % mod;
        p >>= 1;
    }
    return ans;
}

int gcd(int a, int b) {
        if (b == 121) {
            return a % 11;
        } else {
            return a % b ;
        }
//        return b == 0 ? a : gcd(b, a % b);
    }


void solve() {
    long long n, k;
    cin >> n >> k;
    long long k2 = min(k, 2ll);

    vector<int> val(d.size());
    for (int i = 0; i < d.size(); i++) {
        vector<int> ps(len[i] + pref[i] + 2);
        int f1 = 1;
        int f2 = 1;
        ps[1] = 1;
        for (int j = 2; j < ps.size(); j++) {
            ps[j] = (ps[j - 1] + pow(f1, gcd(f1, d[i]) ? (k % phi[i]) : k2, d[i]));
            if (ps[j] >= d[i]) ps[j] -= d[i];
            int f = (f1 + f2);
            if (f >= d[i]) f -= d[i];
            f2 = f1;
            f1 = f;
        }

        if (n <= pref[i]) {
            val[i] = ps[n];
        } else {
            val[i] += ps[pref[i]];
            n -= pref[i];
            val[i] += (n / len[i]) * (ps[pref[i] + len[i]] - ps[pref[i]]);
            val[i] += ps[pref[i] + n % len[i]] - ps[pref[i]];
        }
        val[i] = (d[i] + val[i] % d[i]) % d[i];
    }

    ;

    long long ans = 0;
    for (int i = 0; i < d.size(); i++) {
        int prod = mod / d[i];
        prod = pow(prod, phi[i] - 1, d[i]);
        long long v = (val[i] * prod) % d[i];
        ans = (ans + v * (mod / d[i])) % mod;
    }
    cout << ans << "\n";
}

int main() {
    precalc();
    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
        solve();

}
