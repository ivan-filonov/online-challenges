import java.io.*;
import java.util.*;

public class math_bm {
	FastScanner in;
	PrintWriter out;
	
	// AAAAAAAAAAAAAAAAAAAAAAAAAAA!!! :(

	void solve() {
		int tc = in.nextInt();
		final int mod = (int) 1e9 + 7;
		for (int t = 0; t < tc; t++) {
			int n = in.nextInt();
			int[] a = new int[n * 2];
			for (int i = 0; i < a.length; i++) {
				a[i] = in.nextInt();
			}
			int[][][][][] dp = new int[4][3][n + 1][n + 1][n + 1];
			dp[0][0][0][0][0] = 1;
			for (int su = 0; su <= 2 * n; su++) {
				for (int ph = 0; ph <= n; ph++) {
					for (int ma = 0; ma <= n; ma++) {
						int pa = su - ph - ma;
						if (pa % 2 != 0 || pa < 0) {
							continue;

						}
						pa /= 2;
						if (pa > n) {
							continue;
						}
						for (int la = 0; la < 3; la++) {
							for (int pr = 0; pr < 4; pr++) {
								int pos = pa * 2 + ma + ph;
								int cur = dp[pr][la][ph][ma][pa];
								if (cur == 0) {
									continue;
								}
								if (ph > 0) {
									int cnt = la == 1 ? ph - 1 : ph;
									if (ma + ph + pa * 2 > 1 && pr == 1
											|| a[pos] == 2) {
										cnt = 0;
									}
									int nextPr = (pr * 2) & 3;
									dp[nextPr][0][ph - 1][ma][pa + 1] = (int) ((dp[nextPr][0][ph - 1][ma][pa + 1] + cnt
											* 1L * cur) % mod);
								}
								if (ma > 0) {
									int cnt = la == 2 ? ma - 1 : ma;
									if (ma + ph + pa * 2 > 1 && pr == 2
											|| a[pos] == 1) {
										cnt = 0;
									}
									int nextPr = (pr * 2 + 1) & 3;
									dp[nextPr][0][ph][ma - 1][pa + 1] = (int) ((dp[nextPr][0][ph][ma - 1][pa + 1] + cnt
											* 1L * cur) % mod);
								}
								if (ma + pa + ph < n) {
									int cnt = n - pa - ma - ph;
									int nextLa = 2;
									if (ma + ph + pa * 2 > 1 && pr == 1
											|| a[pos] == 2) {
										cnt = 0;
									}
									int nextPr = (pr * 2) & 3;
									dp[nextPr][nextLa][ph][ma + 1][pa] = (int) ((dp[nextPr][nextLa][ph][ma + 1][pa] + cnt
											* 1L * cur) % mod);
								}
								if (ph + pa + ma < n) {
									int cnt = n - pa - ph - ma;
									int nextLa = 1;
									if (ma + ph + pa * 2 > 1 && pr == 2
											|| a[pos] == 1) {
										cnt = 0;
									}
									int nextPr = (pr * 2 + 1) & 3;
									dp[nextPr][nextLa][ph + 1][ma][pa] = (int) ((dp[nextPr][nextLa][ph + 1][ma][pa] + cnt
											* 1L * cur) % mod);
								}
							}
						}
					}
				}
			}
			long ans = 0;
			for (int pr = 0; pr < 4; pr++) {
				ans += dp[pr][0][0][0][n];
			}
			out.println(ans % mod);
		}
	}

	void run() {
		try {
			in = new FastScanner(new File("math.in"));
			out = new PrintWriter(new File("math.out"));

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
		new math_bm().runIO();
	}
}