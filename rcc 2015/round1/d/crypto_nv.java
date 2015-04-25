import java.util.*;
import java.io.*;

public class crypto_nv {
	FastScanner in;
	PrintWriter out;

	class Polynom {
		int a, b, c;

		public Polynom(int a, int b, int c) {
			super();
			this.a = a;
			this.b = b;
			this.c = c;
		}

		public String get(int x) {
			return Integer.toString(a * x * x + b * x + c);
		}

		@Override
		public String toString() {
			return a + " " + b + " " + c;
		}
	}

	HashMap<String, Integer> hm = new HashMap<String, Integer>();
	StringBuilder sb = new StringBuilder();
	final long MOD = 1_000_000_007L;

	char[] s;
	int n;
	boolean[][] matching;
	long[][][] tree;
	String[] pattetn = new String[10];

	boolean match(int pos, int j) {
		for (int i = 0; i < pattetn[j].length(); i++) {
			if (s[pos + i] != pattetn[j].charAt(i)) {
				return false;
			}
		}
		return true;
	}

	long[] dp = new long[100];

	void make(int pos, int l, int r, int z) {
		if ((z < l || z > r) && (z != -1)) {
			return;
		}
		if (r - l <= 8) {
			tree[pos] = new long[3][3];
			int len = r - l + 1;
			int R = Math.min(2, len);
			for (int L = 0; L <= R; L++) {
				for (int i = 0; i <= len; i++)
					dp[i] = 0;
				dp[L] = 1;
				for (int i = 0; i < len; i++) {
					for (int j = 0; j < 10; j++) {
						if (matching[l + i][j]) {
							dp[i + pattetn[j].length()] += dp[i];
						}
					}
				}
				for (int i = L; i <= len; i++) {
					if (len - i < 3) {
						tree[pos][L][len - i] = dp[i] % MOD;
					}
				}

			}
			return;
		}
		int m = (l + r) >> 1;
		int ls = pos * 2, rs = pos * 2 + 1;
		make(ls, l, m, z);
		make(rs, m + 1, r, z);
		tree[pos] = new long[3][3];

		for (int lr = 1; lr < 3; lr++) {
			for (int rl = 1; rl < 3; rl++) {
				if (rl + lr > 3)
					continue;

				for (int i = m - lr + 1; i <= m + rl; i++) {
					sb.append(s[i]);
				}
				Integer cnt1 = hm.get(sb.toString());
				sb.setLength(0);
				int cnt = cnt1 == null ? 0 : cnt1;
				for (int ll = 0; ll < 3; ll++) {
					for (int rr = 0; rr < 3; rr++) {
						tree[pos][ll][rr] += tree[ls][ll][lr] * cnt
								* tree[rs][rl][rr];
					}
				}
			}
		}

		for (int ll = 0; ll < 3; ll++) {
			for (int rr = 0; rr < 3; rr++) {
				tree[pos][ll][rr] = (tree[pos][ll][rr] + tree[ls][ll][0]
						* tree[rs][0][rr])
						% MOD;
			}
		}
	}

	public void solve() throws IOException {
		Polynom poly = new Polynom(in.nextInt(), in.nextInt(), in.nextInt());
		for (int i = 0; i < 10; i++) {
			pattetn[i] = poly.get(i);
			if (!hm.containsKey(pattetn[i])) {
				hm.put(pattetn[i], 1);
			} else {
				hm.put(pattetn[i], hm.get(pattetn[i]) + 1);
			}
		}

		s = in.next().toCharArray();
		n = s.length;
		matching = new boolean[n + 2][10];
		for (int pos = 0; pos < n; pos++) {
			for (int j = 0; j < 10; j++) {
				if (pos + pattetn[j].length() <= n) {
					matching[pos][j] = match(pos, j);
				}
			}
		}
		tree = new long[4 * n + 1][3][3];

		make(1, 0, n - 1, -1);

		out.println(tree[1][0][0]);
		int m = in.nextInt();
		for (int t = 0; t < m; t++) {
			int p = in.nextInt(), v = in.nextInt();
			s[p - 1] = (char) ('0' + v);
			for (int i = Math.max(0, p - 3); i < Math.min(n, p + 3); i++) {
				for (int j = 0; j < 10; j++) {
					if (i + pattetn[j].length() <= n) {
						matching[i][j] = match(i, j);
					}
				}
			}

			make(1, 0, n - 1, p - 1);
			out.println(tree[1][0][0]);
		}
	}

	public void run() {
		try {
			in = new FastScanner();
			out = new PrintWriter(System.out);
			solve();
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	class FastScanner {
		BufferedReader br;
		StringTokenizer st;

		FastScanner() {
			br = new BufferedReader(new InputStreamReader(System.in));
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

		long nextLong() {
			return Long.parseLong(next());
		}
	}

	public static void main(String[] arg) {
		new crypto_nv().run();
	}
}
