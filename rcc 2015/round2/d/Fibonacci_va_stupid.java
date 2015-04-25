import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.StringTokenizer;

/**
 * Created by Aksenov239 on 18.04.2015.
 */
public class fibonacci_va_stupid {
    public static void main(String[] args) {
        new fibonacci_va_stupid().run();
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

    int mod = 1000000023;

    public int pow(long x, int p, int mod) {
        long ans = 1;
        while (p > 0) {
            if ((p & 1) == 1) {
                ans = (ans * x) % mod;
            }
            x = (x * x) % mod;
            p >>= 1;
        }
        return (int) ans;
    }

    public void solve() throws IOException {
        int n = nextInt();
        int k = nextInt();

        int f1 = 1;
        int f2 = 1;
        long ans = 1;
        for (int j = 2; j <= n; j++) {
            ans = (ans + pow(f1, k, mod)) % mod;
            int f = (f1 + f2) % mod;
            f2 = f1;
            f1 = f;
        }
        //         [1, 101, 40, 17634]
//// );

        out.println(ans);
    }

    public void run() {
        try {
            br = new BufferedReader(new InputStreamReader(System.in));
            out = new PrintWriter(System.out);

            int t = nextInt();
            for (int i = 0; i < t; i++)
                solve();

            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

