import java.io.*;
import java.util.*;

public class Validate {
    private static final int MAX_N = 1000;
    private static final int MAX_C = 1_000_000_000;

    public void run() {
        StrictScanner inf = new StrictScanner(System.in);
        int tests = inf.nextInt();
        ensureLimits(tests, 1, MAX_N, "tests");
        inf.nextLine();
        int sum = 0;
        for (int test = 0; test < tests; test++) {
            int n = inf.nextInt();
            ensureLimits(n, 1, MAX_N, "test " + test + ":" + "n");
            sum += n;
            ensureLimits(sum, 1, MAX_N, "sum of n");
            inf.nextLine();
            HashSet<String> st = new HashSet<String>();
            for (int i = 0; i < n; i++) {
                int x = inf.nextInt();
                int y = inf.nextInt();
                ensureLimits(x, -MAX_C, MAX_C, "test " + test + ":" + "x[" + (i + 1) + "]");
                ensureLimits(y, -MAX_C, MAX_C, "test " + test + ":" + "y[" + (i + 1) + "]");
                inf.nextLine();
                String s = x + "|" + y;
                if (st.contains(s)) {
                    ensure(false, "test " + test + ":" + "point " + (i + 1) + " is dublicate");
                }
                st.add(s);
            }
        }
        inf.close();
    }
    
    public static void main(String[] args) {
        new Validate().run();
    }

    public class StrictScanner {
        private final BufferedReader in;
        private String line = "";
        private int pos;
        private int lineNo;

        public StrictScanner(InputStream source) {
            in = new BufferedReader(new InputStreamReader(source));
            nextLine();
        }

        public void close() {
            ensure(line == null, "Extra data at the end of file");
            try {
                in.close();
            } catch (IOException e) {
                throw new AssertionError("Failed to close with " + e);
            }
        }

        public void nextLine() {
            ensure(line != null, "EOF");
            ensure(pos == line.length(),  "Extra characters on line " + lineNo);
            try {
                line = in.readLine();
            } catch (IOException e) {
                throw new AssertionError("Failed to read line with " + e);
            }
            pos = 0;
            lineNo++;
        }

        public String next() {
            ensure(line != null, "EOF");
            ensure(line.length() > 0,  "Empty line " + lineNo);
            if (pos == 0)
                ensure(line.charAt(0) > ' ',  "Line " + lineNo + " starts with whitespace");
            else {
                ensure(pos < line.length(),  "Line " + lineNo + " is over");
                ensure(line.charAt(pos) == ' ',  "Wrong whitespace on line " + lineNo);
                pos++;
                ensure(pos < line.length(),  "Line " + lineNo + " is over");
                ensure(line.charAt(0) > ' ',  "Line " + lineNo + " has double whitespace");
            }
            StringBuilder sb = new StringBuilder();
            while (pos < line.length() && line.charAt(pos) > ' ')
                sb.append(line.charAt(pos++));
            return sb.toString();
        }

        public int nextInt() {
            String s = next();
            ensure(s.length() == 1 || s.charAt(0) != '0',  "Extra leading zero in number " + s + " on line " + lineNo);
            ensure(s.charAt(0) != '+',  "Extra leading '+' in number " + s + " on line " + lineNo);
            try {
                return Integer.parseInt(s);
            } catch (NumberFormatException e) {
                throw new AssertionError("Malformed number " + s + " on line " + lineNo);
            }
        }

        public long nextLong() {
            String s = next();
            ensure(s.length() == 1 || s.charAt(0) != '0',
                    "Extra leading zero in number " + s + " on line " + lineNo);
            ensure(s.charAt(0) != '+', "Extra leading '+' in number " + s
                    + " on line " + lineNo);
            try {
                return Long.parseLong(s);
            } catch (NumberFormatException e) {
                throw new AssertionError("Malformed number " + s + " on line "
                        + lineNo);
            }
        }

        public double nextDouble() {
            String s = next();
            ensure(s.length() == 1 || s.startsWith("0.") || s.charAt(0) != '0',  "Extra leading zero in number " + s + " on line " + lineNo);
            ensure(s.charAt(0) != '+',  "Extra leading '+' in number " + s + " on line " + lineNo);
            try {
                return Double.parseDouble(s);
            } catch (NumberFormatException e) {
                throw new AssertionError("Malformed number " + s + " on line " + lineNo);
            }
        }
    }

    void ensure(boolean b, String message) {
        if (!b) {
            throw new AssertionError(message);
        }
    }
    
    void ensureLimits(int n, int from, int to, String name) {
        ensure(from <= n && n <= to, name + " must be from " + from + " to " + to);
    }

    void ensureLimits(long n, long from, long to, String name) {
        ensure(from <= n && n <= to, name + " must be from " + from + " to " + to);
    }
}
