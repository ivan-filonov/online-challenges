import java.io.*;
import java.util.*;

public class reflections_iz implements Runnable {
    static public int gcd(int a, int b) {
        a = Math.abs(a);
        b = Math.abs(b);
        while (b > 0) {
            a %= b;
            int o = b;
            b = a;
            a = o;
        }
        return a;
    }

    class Point {
        int x, y;

        Point() {}

        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        void read() {
            this.x = in.nextInt();
            this.y = in.nextInt();
        }

        @Override
        public String toString() {
            return x + " " + y;
        }

        public boolean equals(Point b) {
            return x == b.x && y == b.y;
        }

        public Point subtract(Point b) {
            return new Point(x - b.x, y - b.y);
        }

        public Point add(Point b) {
            return new Point(x + b.x, y + b.y);
        }

        public long scalarProduct(Point b) {
            return (long)x * b.x + (long)y * b.y;
        }

        public long vectorProduct(Point b) {
            return (long)x * b.y - (long)y * b.x;
        }

        public Point multiply(int d) {
            return new Point(x * d, y * d);
        }

        public int up() {
            if (y > 0 || y == 0 && x > 0) {
                return 1;
            }
            if (y < 0 || y == 0 && x < 0) {
                return -1;
            }
            return 0;
        }

        public Point norm() {
            return this.multiply(this.up());
        }

        public Point gcd(Point b) {
            assert this.vectorProduct(b) == 0;
            if (x == 0) {
                if (y == 0) {
                    return new Point(b.x, b.y);
                }
                int g = reflections_iz.gcd(y, b.y);
                int o = y / g;
                return new Point(0, y / o);
            }
            int g = reflections_iz.gcd(x, b.x);
            int o = x / g;
            return new Point(x / o, y / o);
        }
    }

    Scanner in;
    PrintWriter out;

    // get new v2
    Point f2(Point v1, Point v2, Point v3) {
        // solve (v1 * a + v3 * b).y = 0
        // v1.y * a + v3.y * b = 0
        // v1.y * 0 + v3.y * 0 = 0
        // a = k * v3.y / g
        // b =-k * v1.y / g
        // k * v3.y / g * v1.x - k * v1.y / g * v3.x -> min
        int g = gcd(v1.y, v3.y);
        int x4 = Math.abs(v3.y / g * v1.x - v1.y / g * v3.x);
        return new Point(gcd(v2.x, x4), 0);
    }

    int gcd(int a, int b, int[] o) {
        if (a == 0) {
            o[0] = 0;
            o[1] = 1;
            return b;
        }
        int[] o2 = new int[2];
        int g = gcd(b % a, a, o2);
        o[0] = o2[1] - b / a * o2[0];
        o[1] = o2[0];
        return g;
    }

    void euclid(int a, int b, int c, int[] o) {
        int g = gcd(a, b, o);
        o[0] *= c / g;
        o[1] *= c / g;
    }

    // get new v1
    Point f1(Point v1, Point v2, Point v3) {
        // v1.y * a + v3.y * b = gcd(v1.y, v3.y)
        int o[] = new int[2];
        int g = gcd(v1.y, v3.y);
        euclid(v1.y, v3.y, g, o);
        long x = (long)v1.x * o[0] + (long)v3.x * o[1];

        x -= (x / v2.x) * v2.x;
        if (x < 0) {
            x += v2.x;
        }
        return new Point((int)x, g);
    }

    boolean can(Point v1, Point v2, Point v) {
        if (v1.vectorProduct(v2) == 0) {
            Point v3 = v1.gcd(v2);
            if (v3.vectorProduct(v) != 0) {
                return false;
            }
            return v3.gcd(v).equals(v3);
        }
        long det = (long)v1.x * v2.y - (long)v1.y * v2.x;
        long det1= (long)v.x * v2.y - (long)v.y * v2.x;
        long det2= (long)v1.x * v.y - (long)v1.y * v.x;
        return det1 % det == 0 && det2 % det == 0;
    }

    String fastSolve(int n, Point a[], Point s, Point f) {
        if (n == 1) {
            return s.equals(f) || a[0].multiply(2).subtract(s).equals(f) ? "YES" : "NO";
        }

        Point v1 = a[1].subtract(a[0]).multiply(2);
        Point v2 = new Point(0, 0);

        // v1 -- (x0, y), x < x0, y -> min, y > 0
        // v2 -- (x, 0), x -> min, x > 0
        for (int i = 2; i < n; i++) {
            Point v3 = a[i].subtract(a[0]).multiply(2).norm();
            if (v2.x == 0 && v2.y == 0 && v1.vectorProduct(v3) == 0) {
                v1 = v1.gcd(v3);
                continue;
            }
            Point g2 = f2(v1, v2, v3);
            Point g1 = f1(v1, g2, v3);
            v1 = g1;
            v2 = g2;
        }
        boolean ok = can(v1, v2, f.subtract(s));
        for (int i = 0; i < n; i++) {
            ok |= can(v1, v2, f.subtract(a[i].multiply(2).subtract(s)));
        }
        return ok ? "YES" : "NO";
    }

    public void solve() {
        int n = in.nextInt();
        Point a[] = new Point[n];
        for (int i = 0; i < n; i++) {
            a[i] = new Point();
            a[i].read();
        }
        Point s = new Point();
        s.read();
        Point f = new Point();
        f.read();

        out.println(fastSolve(n, a, s, f));
    }

    public void run() {
        InputStream inputStream = System.in;
        OutputStream outputStream = System.out;
        in = new Scanner(inputStream);
        out = new PrintWriter(outputStream);

        int T = in.nextInt();
        while (T-- > 0) {
            solve();
        }

        out.close();
    }

    public static void main(String[] arg) {
        new Thread(null, new reflections_iz(), "", 1 << 26).start();
    }
}


class Scanner {
    BufferedReader in;
    StringTokenizer tok;

    public Scanner(InputStream in) {
        this.in = new BufferedReader(new InputStreamReader(in));
        tok = new StringTokenizer("");
    }

    private String tryReadNextLine() {
        try {
            return in.readLine();
        } catch (Exception e) {
            throw new InputMismatchException();
        }
    }

    public String nextToken() {
        while (!tok.hasMoreTokens()) {
            tok = new StringTokenizer(next());
        }
        return tok.nextToken();
    }

    public String next() {
        String newLine = tryReadNextLine();
        if (newLine == null)
            throw new InputMismatchException();
        return newLine;
    }

    public int nextInt() {
        return Integer.parseInt(nextToken());
    }

}
