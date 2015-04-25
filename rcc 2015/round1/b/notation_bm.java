import java.io.*;
import java.util.*;

public class notation_bm {
	FastScanner in;
	PrintWriter out;

	void solve() {
		int tc = in.nextInt();
		for (int t = 0; t < tc; t++) {
			String a = in.next();
			String b = in.next();
			String c = in.next();
			int[] res = new int[Math.max(c.length(), a.length() + b.length()
					+ 10)];
			for (int i = 0; i < a.length(); i++) {
				for (int j = 0; j < b.length(); j++) {
					int mul = (a.charAt(a.length() - 1 - i) - '0')
							* (b.charAt(b.length() - 1 - j) - '0');
					res[i + j] += mul;
				}
			}
			boolean inf = true;
			for (int i = 0; i < res.length; i++) {
				int val = i >= c.length() ? 0
						: (c.charAt(c.length() - 1 - i) - '0');
				if (val != res[i]) {
					inf = false;
				}
			}
			out.println(inf ? "Infinity" : "Finite");
		}
	}

	void run() {
		try {
			in = new FastScanner(new File("notation.in"));
			out = new PrintWriter(new File("notation.out"));

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
		new notation_bm().run();
	}
}