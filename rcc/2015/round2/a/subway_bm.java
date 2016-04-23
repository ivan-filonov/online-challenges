import java.io.*;
import java.util.*;

public class subway_bm {
    FastScanner in;
    PrintWriter out;

    int get(int x) {
        return x > 99 ? 99 : x;
    }

    void solve() {
        int tc = in.nextInt();
        for (int t = 0; t < tc; t++) {
            int a = in.nextInt();
            int b = in.nextInt();
            int k = in.nextInt();
            int res = -1;
            int time = 0;
            while (a > 0 && b > 0) {
                if (get(a) == get(b) * k || get(a) * k == get(b)) {
                    res = time;
                    break;
                }
                int addTime = 1;
                if (a > 99 && b > 99) {
                    addTime = Math.min(a - 99, b - 99);
                }
                a -= addTime;
                b -= addTime;
                time += addTime;
            }
            out.println(res);
        }
    }

    void run() {
        try {
            in = new FastScanner(new File("subway.in"));
            out = new PrintWriter(new File("subway.out"));

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
        new subway_bm().runIO();
    }
}