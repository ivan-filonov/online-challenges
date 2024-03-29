import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.*;

public class Validate {
    final long MAXN = 1_000_000_000_000_000l; //10^15
    final int MAXT = 10_000;

    public void run() {
        StrictScanner inf = new StrictScanner(System.in);
        int t = inf.nextInt();
        inf.nextLine();
        ensureLimits(t, 1, MAXT, "Incorrect number of tests");
        for (int i = 0; i < t; i++) {
            long a = inf.nextLong();
            long b = inf.nextLong();
            ensureLimits(a, 1, MAXN, "Incorrect a in test #" + String.valueOf(i + 1));
            ensureLimits(b, a, MAXN, "Incorrect b in test #" + String.valueOf(i + 1));
            inf.nextLine();
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
            ensure(pos == line.length(), "Extra characters on line " + lineNo);
            try {
                line = in.readLine();
            } catch (IOException e) {
                throw new AssertionError("Failed to read line with " + e);
            }
            pos = 0;
            lineNo++;
        }

        public String getNextLine() {
            String st = "";
            while (!isEoln()) {
                st = st + next();
            }
            return st;
        }


        public boolean isEoln() {
            return (pos == line.length());
        }

        public String next() {
            ensure(line != null, "EOF");
            ensure(line.length() > 0, "Empty line " + lineNo);
            if (pos == 0)
                ensure(line.charAt(0) > ' ', "Line " + lineNo
                        + " starts with whitespace");
            else {
                ensure(pos < line.length(), "Line " + lineNo + " is over");
                ensure(line.charAt(pos) == ' ', "Wrong whitespace on line "
                        + lineNo);
                pos++;
                ensure(pos < line.length(), "Line " + lineNo + " is over");
                ensure(line.charAt(0) > ' ', "Line " + lineNo
                        + " has double whitespace");
            }
            StringBuilder sb = new StringBuilder();
            while (pos < line.length() && line.charAt(pos) > ' ')
                sb.append(line.charAt(pos++));
            return sb.toString();
        }

        //never used function
        public boolean hasNext() {
            return line != null;
        }

        public int nextInt() {
            String s = next();
            ensure(s.length() == 1 || s.charAt(0) != '0',
                    "Extra leading zero in number " + s + " on line " + lineNo);
            ensure(s.charAt(0) != '+', "Extra leading '+' in number " + s
                    + " on line " + lineNo);
            try {
                return Integer.parseInt(s);
            } catch (NumberFormatException e) {
                throw new AssertionError("Malformed number " + s + " on line "
                        + lineNo);
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
            ensure(s.length() == 1 || s.startsWith("0.") || s.charAt(0) != '0',
                    "Extra leading zero in number " + s + " on line " + lineNo);
            ensure(s.charAt(0) != '+', "Extra leading '+' in number " + s
                    + " on line " + lineNo);
            try {
                return Double.parseDouble(s);
            } catch (NumberFormatException e) {
                throw new AssertionError("Malformed number " + s + " on line "
                        + lineNo);
            }
        }
    }

    static void ensure(boolean b, String message) {
        if (!b) {
            throw new AssertionError(message);
        }
    }

    void ensureString(String s, String alph, String text) {
        for (int i = 0; i < s.length(); i++) {
            ensure(alph.indexOf(s.charAt(i)) != -1, text);
        }
    }

    void ensureLimits(long n, long from, long to, String name) {
        ensure(from <= n && n <= to, name + " must be from " + from + " to "
                + to);
    }

    void ensureLimits(int n, int from, int to, String name) {
        ensure(from <= n && n <= to, name + " must be from " + from + " to "
                + to);
    }

    String line(int x) {
        return "Problem in line #" + Integer.toString(x);
    }

}

