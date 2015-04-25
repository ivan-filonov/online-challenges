import java.io.*;
import java.util.*;

public class reflections_bm {
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

	final int[] myMUL = new int[] { -100, -10, -1, 1, 10, 100 };

	void relax(ArrayList<Point> cur) {
		while (true) {
			boolean ch = false;
			for (int i = 0; i < cur.size(); i++) {
				Point now = cur.get(i);
				if (now.x == 0 && now.y == 0) {
					cur.remove(i);
					ch = true;
					break;
				}
				int wasD2 = now.x * now.x + now.y * now.y;
				for (int j = 0; j < cur.size(); j++) {
					if (j != i) {
						for (int mul : myMUL) {
							int nx = now.x + mul * cur.get(j).x;
							int ny = now.y + mul * cur.get(j).y;
							if (nx * 1L * nx + ny * 1L * ny < wasD2) {
								wasD2 = nx * nx + ny * ny;
								ch = true;
								now.x = nx;
								now.y = ny;
							}
						}
					}
				}
			}
			if (!ch) {
				break;
			}
		}
	}

	boolean can(int dx, int dy, ArrayList<Point> vectors) {
		if (vectors.size() == 0) {
			return dx == 0 && dy == 0;
		}
		if (vectors.size() == 1) {
			int myX = vectors.get(0).x;
			int myY = vectors.get(0).y;
			int mul = myX != 0 ? dx / myX : dy / myY;
			return myX * mul == dx && myY * mul == dy;
		}
		if (vectors.size() > 2) {
			throw new AssertionError();
		}
		int x1 = vectors.get(0).x;
		int y1 = vectors.get(0).y;
		int x2 = vectors.get(1).x;
		int y2 = vectors.get(1).y;
		int zz = x1 * y2 - x2 * y1;
		if (zz == 0) {
			throw new AssertionError();
		}
		int B = dy * x1 - dx * y1;
		B /= zz;
		int A = dx * y2 - dy * x2;
		A /= zz;
		return dx == A * x1 + B * x2 && dy == A * y1 + B * y2;
	}

	String mySolveFast(Point start, Point end, Point[] a) {
		int n = a.length;
		if (start.x == end.x && start.y == end.y) {
			return "YES";
		}
		ArrayList<Point> vectors = new ArrayList<>();
		for (int i = 0; i < a.length - 1; i++) {
			vectors.add(new Point(2 * (a[i + 1].x - a[0].x),
					2 * (a[i + 1].y - a[0].y)));
			relax(vectors);
		}
		if (can(end.x - start.x, end.y - start.y, vectors)) {
			return "YES";
		}
		for (int i = 0; i < n; i++) {
			Point now = reflect(start, a[i]);
			if (can(end.x - now.x, end.y - now.y, vectors)) {
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
			out.println(mySolveFast(start, end, a));
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
		new reflections_bm().runIO();
	}
}