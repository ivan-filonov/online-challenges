/**
 * User: Aksenov Vitaly
 * Date: 18.03.15
 * Time: 15:29
 */

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class game_va implements Runnable {
    public static void main(String[] args) {
        new Thread(new game_va()).run();
    }

    BufferedReader br;
    StringTokenizer in;
    PrintWriter out;

    public String nextToken() throws IOException {
        while (in == null || !in.hasMoreTokens()) {
            in = new StringTokenizer(br.readLine());
        }

        return in.nextToken();
    }

    public int nextInt() throws IOException {
        return Integer.parseInt(nextToken());
    }

    public long nextLong() throws IOException {
        return Long.parseLong(nextToken());
    }

    public double nextDouble() throws IOException {
        return Double.parseDouble(nextToken());
    }

    public class BitSet {
        int[] value;
        int id;

        public BitSet(int n, int id) {
            value = new int[(n + 31) / 32];
            this.id = id;
        }

        public void set(int pos) {
            value[pos >> 5] |= 1 << (pos & 31);
        }

        public boolean contains(BitSet bs) {
            for (int i = 0; i < value.length; i++) {
                if ((value[i] & (bs.value[i])) != bs.value[i]) {
                    return false;
                }
            }
            return true;
        }

        public void union(BitSet bs) {
            for (int i = 0; i < value.length; i++) {
                value[i] |= bs.value[i];
            }
        }

        public int bitCount() {
            int res = 0;
            for (int i = 0; i < value.length; i++) {
                res += Integer.bitCount(value[i]);
            }
            return res;
        }
    }

    public void solve() throws IOException {
        int n = nextInt();
        int m = nextInt();

        BitSet state = new BitSet(n, -1);
        BitSet nstate = new BitSet(n, -1);
        String s = nextToken();
        for (int i = 0; i < n; i++) {
            if (s.charAt(i) == '1') {
                state.set(i);
            } else {
                nstate.set(i);
            }
        }

        ArrayList<BitSet> on = new ArrayList<BitSet>();
        ArrayList<BitSet> off = new ArrayList<BitSet>();
        for (int i = 0; i < m; i++) {
            BitSet bs = new BitSet(n, i);
            int type = nextInt();
            s = nextToken();
            for (int j = 0; j < n; j++) {
                if (s.charAt(j) == '1') {
                    bs.set(j);
                }
            }

            if (type == 1) {
                on.add(bs);
            } else {
                off.add(bs);
            }
        }

        boolean[] take = new boolean[n];
        int[] ans = new int[n];
        int l = 0;
        for (int i = 0; i < n; i++) {
            for (BitSet bs : on) {
                if (!take[bs.id] && state.contains(bs)) {
                    nstate.union(bs);
                    ans[l++] = bs.id + 1;
                    take[bs.id] = true;
                }
            }
            for (BitSet bs : off) {
                if (!take[bs.id] && nstate.contains(bs)) {
                    state.union(bs);
                    ans[l++] = bs.id + 1;
                    take[bs.id] = true;
                }
            }
        }

        if (nstate.bitCount() == n) {
            out.println("YES");
            out.println(l);
            for (int i = l - 1; i >= 0; i--) {
                out.print(ans[i] + " ");
            }
        } else {
            out.println("NO");
        }

    }

    public void run() {
        try {
            br = new BufferedReader(new InputStreamReader(System.in));
            out = new PrintWriter(System.out);

            solve();

            out.close();
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }
    }
}

