import java.io.*;
import java.util.*;

public class Validate {
	private static final int MAX_LEN = 50_000;
	private static final int MAX_M = 50_000;

	class Polynom {
		int a, b, c;

		public Polynom(int a, int b, int c) {
			super();
			this.a = a;
			this.b = b;
			this.c = c;
		}

		public int get(int x) {
			return a * x * x + b * x + c;
		}
	}

	public void run() {
		StrictScanner inf = new StrictScanner(System.in);
		int a = inf.nextInt(), b = inf.nextInt(), c = inf.nextInt();
		Polynom poly = new Polynom(a, b, c);
		ensureLimits(a, 0, 10, "a");
		ensureLimits(a, -10, 10, "b");
		ensureLimits(a, -10, 10, "c");
		inf.nextLine();
		for (int x = 0; x < 10; x++) {
			ensure(poly.get(x) >= 0, "Polynom must not negative, but at x = "
					+ x + " polynom value " + poly.get(x));
		}
		String s = inf.next();
		inf.nextLine();
		ensureLimits(s.length(), 1, MAX_LEN, "Length string");

		int m = inf.nextInt();
		inf.nextLine();
		ensureLimits(m, 0, MAX_M, "m");
		for (int i = 0; i < m; i++ ) {
			int pos = inf.nextInt(), dig = inf.nextInt();
			inf.nextLine();
			ensureLimits(pos, 1, s.length(), "Position at line " + (i + 1));
			ensureLimits(dig, 0, 9, "digit");
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

	void ensure(boolean b, String message) {
		if (!b) {
			throw new AssertionError(message);
		}
	}

	void ensureLimits(int n, int from, int to, String name) {
		ensure(from <= n && n <= to, name + " must be from " + from + " to "
				+ to);
	}

	void ensureLimits(long n, long from, long to, String name) {
		ensure(from <= n && n <= to, name + " must be from " + from + " to "
				+ to);
	}
}

