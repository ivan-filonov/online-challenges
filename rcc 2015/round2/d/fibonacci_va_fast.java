import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.HashMap;
import java.util.StringTokenizer;

/**
 * Created by Aksenov239 on 18.04.2015.
 */
public class fibonacci_va_fast {
    public static void main(String[] args) {
        new fibonacci_va_fast().run();
    }

    BufferedReader br;
    StringTokenizer in;
    PrintWriter out;

    public String nextToken() throws IOException {
        while (in == null || !in.hasMoreTokens())
            in = new StringTokenizer(br.readLine());
        return in.nextToken();
    }

    public int nextInt() throws IOException {
        return Integer.parseInt(nextToken());
    }

    public long nextLong() throws IOException {
        return Long.parseLong(nextToken());}

    int[] d;
    int[] phi;
    int[] pref;
    int[] len;
    int mod = 1000000023;//100000000;

    public void precalc() {
        int x = mod;
        d = new int[10];
        phi = new int[10];
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

        d = Arrays.copyOf(d, l);
        phi = Arrays.copyOf(phi, l);

        len = new int[l];
        pref = new int[l];

        for (int i = 0; i < l; i++) {
            int f1 = 1;
            int f2 = 1;
            HashMap<Long, Integer> hm = new HashMap<Long, Integer>();
            int j = 0;
            while (true) {
                long pair = 1L * f1 * d[i] + f2;
                if (hm.containsKey(pair)) {
                    len[i] = j - hm.get(pair);
                    pref[i] = hm.get(pair);
                    break;
                }
                hm.put(pair, j);

                int f = (f1 + f2) % d[i];
                f2 = f1;
                f1 = f;
                j++;
            }
        }
    }

    public int gcd(int a, int b) {
        if (b == 121) {
            return a % 11;
        } else {
            return a % b ;
        }
//        return b == 0 ? a : gcd(b, a % b);
    }

    public int pow(int x, long p, int mod) {
        x = x % mod;
        if (x % mod == 0) {
          return 0;
        }
        int ans = 1;
        while (p > 0) {
            if ((p & 1) == 1) {
                ans = (ans * x) % mod;
            }
            x = (x * x) % mod;
            p >>= 1;
        }
        return ans;
    }

    public void solve() throws IOException {
        int n = nextInt(); 
        long k = nextLong();

        int[] val = new int[d.length];
        for (int i = 0; i < d.length; i++) {
            int[] ps = new int[len[i] + pref[i] + 2];
            int f1 = 1;
            int f2 = 1;
            ps[1] = 1;
            for (int j = 2; j < ps.length; j++) {
                ps[j] = (ps[j - 1] + pow(f1, gcd(f1, d[i]) != 0 ? k % phi[i] : k, d[i]));
                if (ps[j] >= d[i]) {
                    ps[j] -= d[i];
                }
                int f = (f1 + f2);
                if (f >= d[i]) {
                    f -= d[i];
                }
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

        long ans = 0;
        for (int i = 0; i < d.length; i++) {
            int prod = mod / d[i];
            prod = pow(prod, phi[i] - 1, d[i]);
            long v = (val[i] * prod) % d[i];
            ans = (ans + v * (mod / d[i])) % mod;
        }
        out.println(ans);
    }

    public void run() {
        try {
            br = new BufferedReader(new InputStreamReader(System.in));
            out = new PrintWriter(System.out);

            precalc();

            int t = nextInt();
            for (int i = 0; i < t; i++)
                solve();

            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

