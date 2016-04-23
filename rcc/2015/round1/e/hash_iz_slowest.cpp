#include <cstdio>
#include <vector>
#include <cassert>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int ALPHABET = 26;
const int maxk = 20;
const int maxn = 1 << maxk;
const int mod = 998244353;
const int root = 31;
const int root_inv = 128805723;
const int root_pw = 1 << 23;

void add(int &a, int b) {
    a += b;
    if (a >= mod) a -= mod;
}

int n, m, p, x;

int slowest_solve() {
    vector<vector<int> > dp(2, vector<int>(m));
    dp[0][0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 1; k <= ALPHABET; k++) {
                dp[1][(1LL * j * p + k) % m] = (dp[1][(1LL * j * p + k) % m] + dp[0][j]) % mod;
            }
        }
        for (int j = 0; j < m; j++) {
            dp[0][j] = dp[1][j];
            dp[1][j] = 0;
        }
    }
    return dp[0][x];
}

int bin(int n, int k, int mod) {
    if (k == 0) return 1 % mod;
    int res = bin(n, k / 2, mod);
    res = 1LL * res * res % mod;
    if (k % 2 == 1) res = 1LL * res * n % mod;
    return res;
}

int _a[maxn], _b[maxn];

int rev(int x, int lg) {
    int res = 0;
    for (int i = 0; i < lg; i++) if (x & (1 << i)) res |= 1 << (lg - i - 1);
    return res;
}

void fft(int *a, int n, bool inv) {
    int lg = 0;
    while ((1 << lg) < n) lg++;

    for (int i = 0; i < n; i++) {
        int j = rev(i, lg);
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 1; len < n; len <<= 1) {
        int w0 = inv ? root_inv : root;
        for (int i = len * 2; i < root_pw; i <<= 1) w0 = 1LL * w0 * w0 % mod;
        for (int i = 0; i < n; i += 2 * len) {
            int w = 1;
            for (int j = 0; j < len; j++) {
                int u = a[i + j], v = 1LL * a[i + j + len] * w % mod;
                a[i + j] = (u + v) % mod;
                a[i + j + len] = (u - v + mod) % mod;
                w = 1LL * w * w0 % mod;
            }
        }
    }
    if (inv) {
        int o = bin(n, mod - 2, mod);
        for (int i = 0; i < n; i++) a[i] = 1LL * a[i] * o % mod;
    }
}

vector<int> multiply(vector<int> a, vector<int> b, bool fast) {
    if (fast == false) {
        vector<int> res(m);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                add(res[(i + j) % m], 1LL * a[i] * b[j] % mod);
            }
        }
        return res;
    } else {
        int n = 1;
        while (n < max(a.size(), b.size())) n <<= 1;
        n <<= 1;
        for (int i = 0; i < n; i++) {
            _a[i] = i < (int)a.size() ? a[i] : 0;
            _b[i] = i < (int)b.size() ? b[i] : 0;
        }
        fft(_a, n, 0);
        fft(_b, n, 0);
        for (int i = 0; i < n; i++) {
            _a[i] = 1LL * _a[i] * _b[i] % mod;
        }
        fft(_a, n, 1);
        vector<int> res(m);
        for (int i = 0; i < n; i++) add(res[i % m], _a[i]);
        return res;
    }
}

vector<int> merge_dp(vector<int> a, vector<int> b, int pw, bool fast) {
    int o = bin(p, pw, m);
    vector<int> na(m);
    for (int i = 0; i < m; i++) add(na[1LL * i * o % m], a[i]);
    a = na;
    return multiply(a, b, fast);
}

int solve(bool fast) {
    vector<vector<int> > dp(maxk, vector<int>(m));
    for (int i = 1; i <= ALPHABET; i++) add(dp[0][i % m], 1);
    for (int i = 1; i < maxk; i++) {
        dp[i] = merge_dp(dp[i - 1], dp[i - 1], (1 << (i - 1)), fast);
    }

    vector<int> res(m, 0);
    res[0] = 1;
    for (int j = maxk - 1; j >= 0; j--) {
        if (!(n & (1 << j))) continue;
        res = merge_dp(res, dp[j], (1 << j), fast);
    }
    return res[x];
}

void gen() {
    n = rand() % 1111 + 1;
    m = rand() % 111 + 2;
    p = rand() % (m - 1) + 1;
    x = rand() % m;
}

void stress(bool f) {
    if (!f) return;
    for (int it = 0;; it++) {
        cerr << it << endl;
        gen();
        int ans1 = solve(true);
        int ans2 = solve(false);
        if (ans1 != ans2) {
            cerr << ans1 << " instead of " << ans2 << endl;
            cerr << n << " " << m << " " << p << " " << x << endl;
            solve(true);
            exit(0);
        }
    }
}

void work(bool f) {
    if (!f) return;
    vector<int> a(2, 1);
    multiply(a, a, 1);
    cout << bin(3, mod - 2, mod) << endl; exit(0);
    for (int x = 3; x <= 3; x++) {
        int cur = x;
        int it = 1;
        //for (int i = 0; i <= 1 << 23; i++) {
        while (cur != 1) {
            cur = 1LL * cur * x % mod;
            it++;
            if (cur == 1) cout << it << endl;
            if (abs((1 << 23) - it) < 10) cout << cur << endl;
        }
        cout << it << endl;
    }
    exit(0);
}

int main() {
    work(0);
    stress(0);
    //freopen("hash.in", "r", stdin);
    //freopen("hash.out", "w", stdout);

    while (scanf("%d%d%d%d", &n, &m, &p, &x) == 4) {
        printf("%d\n", slowest_solve());
    }

    return 0;
}

