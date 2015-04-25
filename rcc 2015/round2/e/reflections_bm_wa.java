import java.io.*;
import java.util.*;

public class reflections_bm_wa {
	FastScanner in;
	PrintWriter out;

	class Point {
		int x, y;

		public Point(int x, int y) {
			super();
			this.x = x;
			this.y = y;
		}

		@Override
		public String toString() {
			return "[x=" + x + ", y=" + y + "]";
		}

	}

	Point reflect(Point a, Point b) {
		return new Point(b.x * 2 - a.x, b.y * 2 - a.y);
	}

	final int MAGIC = 100;
	final int MAX_MAX = 111;
	boolean[][] can = new boolean[MAX_MAX][MAX_MAX];
	int[] qX = new int[MAX_MAX * MAX_MAX];
	int[] qY = new int[MAX_MAX * MAX_MAX];
	int[][] time = new int[MAX_MAX][MAX_MAX];
	int curTime = 0;

	boolean can(int dx, int dy, Point[] mvectors, int MAX) {
		curTime++;
		ArrayList<Point> vectors = new ArrayList<>();
		for (Point p : mvectors) {
			int x = p.x % MAX;
			int y = p.y % MAX;
			if (x < 0) {
				x += MAX;
			}
			if (y < 0) {
				y += MAX;
			}
			if (time[x][y] != curTime) {
				time[x][y] = curTime;
				vectors.add(new Point(x, y));
			}
		}
		for (int i = 0; i < MAX; i++) {
			for (int j = 0; j < MAX; j++) {
				can[i][j] = false;
			}
		}
		can[0][0] = true;
		int needX = ((dx % MAX) + MAX) % MAX, needY = ((dy % MAX) + MAX) % MAX;
		if (can[needX][needY]) {
			return true;
		}
		int qIt = 0, qSz = 1;
		qX[0] = 0;
		qY[0] = 0;
		while (qIt < qSz) {
			int i = qX[qIt], j = qY[qIt++];
			for (Point add : vectors) {
				for (int mul = -1; mul < 2; mul += 2) {
					int nx = i + add.x * mul;
					int ny = j + add.y * mul;
					nx %= MAX;
					ny %= MAX;
					if (nx < 0) {
						nx += MAX;
					}
					if (ny < 0) {
						ny += MAX;
					}
					if (!can[nx][ny]) {
						can[nx][ny] = true;
						qX[qSz] = nx;
						qY[qSz++] = ny;
						if (nx == needX && ny == needY) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	boolean DEBUG = false;

	String mySolve(Point start, Point end, Point[] a) {
		int n = a.length;
		if (start.x == end.x && start.y == end.y) {
			return "YES";
		}
		Point[] vectors = new Point[n - 1];
		for (int i = 0; i < vectors.length; i++) {
			vectors[i] = new Point(2 * (a[i + 1].x - a[0].x),
					2 * (a[i + 1].y - a[0].y));
		}
		{
			boolean ok = true;
			for (int sz = 1; sz <= MAGIC; sz++) {
				if (!can(end.x - start.x, end.y - start.y, vectors, sz)) {
					ok = false;
					break;
				}
			}
			if (ok) {
				return "YES";
			}
		}
		for (int i = 0; i < n; i++) {
			Point now = reflect(start, a[i]);
			boolean ok = true;
			for (int sz = 1; sz <= MAGIC; sz++) {
				if (!can(end.x - now.x, end.y - now.y, vectors, sz)) {
					ok = false;
					break;
				}
			}
			if (ok) {
				return "YES";
			}
		}
		return "NO";
	}

	void solve() {
		int tc = in.nextInt();
		for (int t = 0; t < tc; t++) {
			int n = in.nextInt();
			Point[] a = new Point[n];
			for (int i = 0; i < n; i++) {
				a[i] = new Point(in.nextInt(), in.nextInt());
			}
			Point start = new Point(in.nextInt(), in.nextInt());
			Point end = new Point(in.nextInt(), in.nextInt());
			out.println(mySolve(start, end, a));
		}
	}

	void run() {
		try {
			in = new FastScanner(new File("reflections.in"));
			out = new PrintWriter(new File("reflections.out"));

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
		new reflections_bm_wa().runIO();
	}
}