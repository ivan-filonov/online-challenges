import java.io.*;
import java.util.*;

public class game_bm {
	FastScanner in;
	PrintWriter out;

	void solve() {
		int tc = in.nextInt();
		for (int t = 0; t < tc; t++) {
			int n = in.nextInt();
			int[][] cost = new int[n][];
			for (int i = 0; i < n; i++) {
				cost[i] = new int[i * 2 + 2];
				for (int j = 0; j < i * 2 + 2; j++) {
					cost[i][j] = in.nextInt();
				}
			}
			double[][] res = new double[n][];
			for (int i = n - 1; i >= 0; i--) {
				res[i] = new double[i + 1];
				for (int j = 0; j < res[i].length; j++) {
					if (i == n - 1) {
						res[i][j] = Math
								.min(cost[i][j * 2], cost[i][j * 2 + 1]);
					} else {
						if (cost[i][j * 2] < cost[i][j * 2 + 1]) {
							res[i][j] = cost[i][j * 2] + res[i + 1][j];
						} else {
							if (cost[i][j * 2] > cost[i][j * 2 + 1]) {
								res[i][j] = cost[i][j * 2 + 1]
										+ res[i + 1][j + 1];
							} else {
								res[i][j] = cost[i][j * 2]
										+ (res[i + 1][j + 1] + res[i + 1][j])
										/ 2;
							}
						}
					}
				}
			}
			out.printf(Locale.US, "%.17f\n", res[0][0]);
		}
	}

	void run() {
		try {
			in = new FastScanner(new File("game.in"));
			out = new PrintWriter(new File("game.out"));

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
		new game_bm().runIO();
	}
}