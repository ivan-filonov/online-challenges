import java.util.Scanner;

public class hash_iz_java {
    private static final int ALPHABET = 26;
    private static final int MOD = 998244353;
    private static final int ROOT = 31;
    private static final int ROOT_INV = 128805723;
    private static final int ROOT_PW = 1 << 23;

    int n, m, p, x;

    int bin(int n, int k, int mod) {
        if (k == 0) {
            return 1 % mod;
        }
        int res = bin(n, k / 2, mod);
        res = (int)(1L * res * res % mod);
        if (k % 2 == 1) {
            res = (int)(1L * res * n % mod);
        }
        return res;
    }

    int rev(int x, int lg) {
        int res = 0;
        for (int i = 0; i < lg; i++) {
            if ((x & (1 << i)) != 0) {
                res |= 1 << (lg - i - 1);
            }
        }
        return res;
    }

    void fft(int[] a, int n, boolean inv) {
        int lg = 0;
        while ((1 << lg) < n) {
            lg++;
        }

        for (int i = 0; i < n; i++) {
            int j = rev(i, lg);
            if (i <= j) {
                int o = a[i];
                a[i] = a[j];
                a[j] = o;
            }
        }

        for (int len = 1; len < n; len *= 2) {
            int w0 = inv ? ROOT_INV : ROOT;
            for (int i = len * 2; i < ROOT_PW; i *= 2) {
                w0 = (int)(1L * w0 * w0 % MOD);
            }
            for (int i = 0; i < n; i += 2 * len) {
                int w = 1;
                for (int j = 0; j < len; j++) {
                    int u = a[i + j];
                    int v = (int)(1L * a[i + j + len] * w % MOD);
                    a[i + j] = (u + v) % MOD;
                    a[i + j + len] = (u - v + MOD) % MOD;
                    w = (int)(1L * w * w0 % MOD);
                }
            }
        }

        if (inv) {
            int o = bin(n, MOD - 2, MOD);
            for (int i = 0; i < n; i++) {
                a[i] = (int)(1L * a[i] * o % MOD);
            }
        }
    }

    int[] multiply(int[] _a, int[] _b) {
        int n = 1;
        while (n < Math.max(_a.length, _b.length)) {
            n *= 2;
        }
        n *= 2;

        int[] a = new int[n];
        int[] b = new int[n];
        for (int i = 0; i < n; i++) {
            if (i < _a.length) {
                a[i] = _a[i];
            }
            if (i < _b.length) {
                b[i] = _b[i];
            }
        }
        fft(a, n, false);
        fft(b, n, false);
        for (int i = 0; i < n; i++) {
            a[i] = (int)(1L * a[i] * b[i] % MOD);
        }
        fft(a, n, true);
        int[] res = new int[m];
        for (int i = 0; i < n; i++) {
            res[i % m] += a[i];
            res[i % m] %= MOD;
        }
        return res;
    }

    int[] merge(int[] a, int[] b, int pw) {
        int o = bin(p, pw, m);
        int[] na = new int[m];
        for (int i = 0; i < m; i++) {
            na[(int)(1L * i * o % m)] += a[i];
            na[(int)(1L * i * o % m)] %= MOD;
        }
        return multiply(na, b);
    }

    void run() {
        Scanner in = new Scanner(System.in);
        n = in.nextInt();
        m = in.nextInt();
        p = in.nextInt();
        x = in.nextInt();

        int[][] dp = new int[20][m];
        for (int j = 1; j <= ALPHABET; j++) {
            dp[0][j % m] = (dp[0][j % m] + 1) % MOD;
        }
        for (int i = 1; i < 20; i++) {
            dp[i] = merge(dp[i - 1], dp[i - 1], 1 << (i - 1));
        }

        int[] res = new int[m];
        res[0] = 1 % MOD;
        for (int i = 0; i < 20; i++) {
            if ((n & (1 << i)) == 0) {
                continue;
            }
            res = merge(res, dp[i], 1 << i);
        }
        System.out.println(res[x]);
    }

    public static void main(String[] args) {
        new hash_iz_java().run();
    }
}

