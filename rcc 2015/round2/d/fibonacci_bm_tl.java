import java.io.*;
import java.util.*;

public class fibonacci_bm_tl {
    FastScanner in;
    PrintWriter out;

    // I don't know math, that's the reason why this solution gets TL
    // It can solve one testcase in ok time, but not 100 of them :(

    final int MAGIC = 1490280;
    final int mod = (int) 1e9 + 23;

    int pow(int x, long y) {
        if (y == 0) {
            return 1;
        }
        long z = pow(x, y / 2);
        z = z * z % mod;
        if (y % 2 == 1) {
            z = z * x % mod;
        }
        return (int) z;
    }

    void solve() {
        int[] fib = new int[MAGIC];
        fib[0] = fib[1] = 1;
        for (int i = 2; i < fib.length; i++) {
            fib[i] = fib[i - 1] + fib[i - 2];
            if (fib[i] >= mod) {
                fib[i] -= mod;
            }
        }
        int tc = in.nextInt();
        for (int t = 0; t < tc; t++) {
            int n = in.nextInt();
            long k = in.nextLong();
            long res = 0;
            int cnt = Math.min(n, fib.length);
            int cntFull = n / fib.length;
            int more = n % fib.length;
            for (int i = 0; i < cnt; i++) {
                int mul = i < more ? cntFull + 1 : cntFull;
                res = (res + mul * 1L * pow(fib[i], k)) % mod;
            }
            out.println(res);
        }
    }

    void run() {
        try {
            in = new FastScanner(new File("fibonacci.in"));
            out = new PrintWriter(new File("fibonacci.out"));

            solve();

            out.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    void runIO() {

        in = new FastScanner(System.in);
        out = new PrintWriter(System.out);

        solve();

        out.close();
    }

    class FastScanner {
        BufferedReader br;
        StringTokenizer st;

        public FastScanner(File f) {
            try {
                br = new BufferedReader(new FileReader(f));
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
        }

        public FastScanner(InputStream f) {
            br = new BufferedReader(new InputStreamReader(f));
        }

        String next() {
            while (st == null || !st.hasMoreTokens()) {
                String s = null;
                try {
                    s = br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                if (s == null)
                    return null;
                st = new StringTokenizer(s);
            }
            return st.nextToken();
        }

        boolean hasMoreTokens() {
            while (st == null || !st.hasMoreTokens()) {
                String s = null;
                try {
                    s = br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                if (s == null)
                    return false;
                st = new StringTokenizer(s);
            }
            return true;
        }

        int nextInt() {
            return Integer.parseInt(next());
        }

        long nextLong() {
            return Long.parseLong(next());
        }

        double nextDouble() {
            return Double.parseDouble(next());
        }
    }

    public static void main(String[] args) {
        new fibonacci_bm_tl().runIO();
    }
}