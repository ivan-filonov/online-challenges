import java.io.*;
import java.util.*;

public class crypto_bm {
	FastScanner in;
	PrintWriter out;

	final int mod = (int) 1e9 + 7;

	class SegmentTree {
		int[] vals;
		int[][][] ways;
		int n;

		SegmentTree(String s) {
			n = s.length();
			vals = new int[n];
			ways = new int[3][3][n * 4];
			build(0, 0, n - 1, s);
		}

		void build(int v, int l, int r, String s) {
			if (l == r) {
				vals[l] = s.charAt(l) - '0';
			} else {
				int m = (l + r) >>> 1;
				build(v * 2 + 1, l, m, s);
				build(v * 2 + 2, m + 1, r, s);
			}
			calc(v, l, r);
		}

		boolean intersect(int l1, int r1, int l2, int r2) {
			return Math.min(r1, r2) >= Math.max(l1, l2);
		}

		void update(int v, int l, int r, int need, int newValue) {
			if (!intersect(l, r, need - 2, need + 2)) {
				return;
			}
			if (l == r) {
				if (l == need) {
					vals[l] = newValue;
				}
			} else {
				int m = (l + r) >>> 1;
				update(v * 2 + 1, l, m, need, newValue);
				update(v * 2 + 2, m + 1, r, need, newValue);
			}
			calc(v, l, r);
		}

		void calc(int v, int l, int r) {
			if (l == r) {
				for (int left = 0; left < 3; left++) {
					for (int right = 0; right < 3; right++) {
						ways[left][right][v] = 0;
						int realFrom = l - left;
						int realTo = r - right;
						if (realFrom >= 0 && realTo >= l) {
							int number = 0;
							for (int i = 0; i < realTo - realFrom + 1; i++) {
								number = number * 10 + vals[realFrom + i];
							}
							if (realTo != realFrom && vals[realFrom] == 0) {

							} else {
								ways[left][right][v] += totalWays[number];
							}
						}
						if (realFrom >= 0 && realFrom == realTo + 1) {
							ways[left][right][v] = 1;
						}
					}
				}
			} else {
				for (int left = 0; left < 3; left++) {
					for (int right = 0; right < 3; right++) {
						ways[left][right][v] = 0;
						for (int cnt = 0; cnt < 3; cnt++) {
							ways[left][right][v] = (int) ((ways[left][right][v] + ways[left][cnt][v * 2 + 1]
									* 1L * ways[cnt][right][v * 2 + 2]) % mod);
						}
					}
				}
			}
			// for (int left = 0; left < 3; left++) {
			// for (int right = 0; right < 3; right++) {
			// System.err.println(v + " " + l + " " + r + " " + left + " "
			// + right + " -> " + ways[left][right][v]);
			// }
			// }
		}
	}

	int[] totalWays;

	void solve() {
		int a = in.nextInt();
		int b = in.nextInt();
		int c = in.nextInt();
		totalWays = new int[1000];
		for (int i = 0; i < 10; i++) {
			totalWays[i * i * a + i * b + c]++;
		}
//		for (int i = 0; i < totalWays.length; i++) {
//			if (totalWays[i] != 0) {
//				System.err.println(i + "-?>" + totalWays[i]);
//			}
//		}
		SegmentTree st = new SegmentTree(in.next());
		out.println(st.ways[0][0][0]);
		int qq = in.nextInt();
		for (int q = 0; q < qq; q++) {
			st.update(0, 0, st.n - 1, in.nextInt() - 1, in.nextInt());
			out.println(st.ways[0][0][0]);
		}
	}

	void run() {
		try {
			in = new FastScanner(new File("crypto.in"));
			out = new PrintWriter(new File("crypto.out"));

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
		new crypto_bm().runIO();
	}
}