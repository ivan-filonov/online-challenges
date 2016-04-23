import java.io.*;
import java.util.*;

public class points_iz_wa2 {
    static final long MX = 2_100_000_000L;

    static class Point {
        int x, y;

        Point() {}
        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }
    }

    static class Pair implements Comparable<Pair> {
        long a, b;

        Pair(long a, long b) {
            this.a = a;
            this.b = b;
        }

        public boolean equals(Pair pt) {
            return a == pt.a && b == pt.b;
        }

        @Override
        public int compareTo(Pair o) {
            if (a != o.a) {
                return Long.compare(a, o.a);
            }
            return Long.compare(b, o.b);
        }
    }

    long gcd(long a, long b) {
        while (b > 0) {
            a %= b;
            long o = a;
            a = b;
            b = o;
        }
        return a;
    }

    Scanner in;
    PrintWriter out;

    void solve() {
        int n = in.nextInt();
        Point a[] = new Point[n];
        for (int i = 0; i < n; i++) {
            a[i] = new Point();
            a[i].x = in.nextInt();
            a[i].y = in.nextInt();
        }

        HashMap<Long, ArrayList<Pair>> map = new HashMap<>();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                long A = a[i].y - a[j].y;
                long B = a[j].x - a[i].x;
                long C = -(A * a[i].x + B * a[i].y);
                long G = gcd(Math.abs(A), Math.abs(B));
                A /= G;
                B /= G;
                if (A < 0 || A == 0 && B < 0) {
                    A *= -1;
                    B *= -1;
                    C *= -1;
                }
                long GG = gcd(Math.abs(C), G);
                C /= GG;
                G /= GG;

                map.putIfAbsent(A * MX + B, new ArrayList<>());
                map.get(A * MX + B).add(new Pair(C, G));
            }
        }

        final int[] ans = {1};
        map.forEach((ignored, v) -> {
            Collections.sort(v);
            // n * (n - 1) / 2 = x
            // n * n - n - 2x = 0

            // n = (1 + sqrt(1 + 8x)) / 2
            int mx1 = -1, mx2 = -1;
            for (int L = 0; L < v.size(); L++) {
                int R = L;
                while (R < v.size() && v.get(R).equals(v.get(L))) {
                    R++;
                }
                int cnt = R - L;
                int c = (int)((1 + Math.sqrt(1.0 + 8 * cnt)) / 2 + 0.1);
                if (c > mx1) {
                    mx2 = mx1;
                    mx1 = c;
                } else if (c > mx2) {
                    mx2 = c;
                }
            }
            if (mx2 == -1) {
                ans[0] = Math.max(ans[0], Math.min(n, mx1 + 1));
            } else {
                ans[0] = Math.max(ans[0], mx1 + mx2);
            }
        });
        out.println(ans[0]);
    }

    public void run() {
        in = new Scanner(System.in);
        out = new PrintWriter(System.out);

        int T = in.nextInt();
        while (T --> 0) {
            solve();
        }
        out.close();
    }

    public static void main(String[] args) {
        new points_iz_wa2().run();
    }
}

class Scanner {
    BufferedReader br;
    StringTokenizer st;

    Scanner(File f) {
        try {
            br = new BufferedReader(new FileReader(f));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    Scanner(InputStream stream) {
        br = new BufferedReader(new InputStreamReader(stream));
    }

    String next() {
        while (st == null || !st.hasMoreTokens()) {
            try {
                st = new StringTokenizer(br.readLine());
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return st.nextToken();
    }

    int nextInt() {
        return Integer.parseInt(next());
    }
}
