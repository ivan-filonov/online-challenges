/**
 * User: Aksenov Vitaly
 * Date: 18.03.15
 * Time: 15:29
 */

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.StringTokenizer;

public class math_va implements Runnable {
    public static void main(String[] args) {
        new Thread(new math_va()).run();
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

    int mod = 1000000007;

    public long solve(int n, int[] a) throws IOException {
        long[][][][] total = new long[2 * n][n + 1][n + 1][4];

        for (int f = 0; f < 2; f++) {
            if ((a[0] & (1 << f)) != 0) {
                for (int s = 0; s < 2; s++) {
                    if (n == 1 && f == s)
                        continue;
                    if ((a[1] & (1 << s)) != 0) {
                        total[1][f + s][f == s ? 0 : 1][2 * f + s] = 1;
                    }
                }
            }
        }


        for (int i = 1; i < 2 * n - 1; i++) {
            for (int math = 0; math <= n; math++) {
                for (int d = 0; d <= n; d++) {
                    for (int mask = 0; mask < 4; mask++) {
                        int closed = ((mask & 1) + (mask >> 1)) % 2 == 0 ? -1 : (mask >> 1);
                        for (int next = 0; next < 2; next++) {
                            if (next != closed && math + next <= n && (a[i + 1] & (1 << next)) != 0 && d + ((mask & 1) == next ? 0 : 1) <= n) {
                                total[i + 1][math + next][d + ((mask & 1) == next ? 0 : 1)][((mask & 1) << 1) + next] += total[i][math][d][mask];
                                total[i + 1][math + next][d + ((mask & 1) == next ? 0 : 1)][((mask & 1) << 1) + next] %= mod;
                            }
                        }
                    }
                }
            }
        }


        long[] dd = new long[n + 1];
        for (int i = 0; i < dd.length; i++) {
            for (int mask = 0; mask < 4; mask++) {
                dd[i] += total[2 * n - 1][n][i][mask];
                dd[i] %= mod;
            }
        }

        long[] f = new long[n + 1];
        f[0] = 1;
        for (int i = 1; i < f.length; i++) {
            f[i] = (f[i - 1] * i) % mod;
        }

        long[][] d = new long[n + 1][n + 1];
        for (int i = 0; i <= n; i++)
            d[i][0] = f[i];
        for (int i = 1; i <= n; i++)
            d[i][1] = ((f[i] - f[i - 1]) % mod + mod) % mod;

        for (int i = 2; i <= n; i++) {
            for (int j = 2; j <= i; j++) {
                d[i][j] = (i - 1) * d[i - 1][j - 1] + (j - 1) * d[i - 2][j - 2];
                d[i][j] %= mod;
            }
        }

        long ans = 0;
        for (int i = 0; i < f.length; i++)
            ans = (ans + dd[i] * d[n][i]) % mod;

        ans = (ans * f[n]) % mod;
        return ans;
    }

    public long solve2(int n, int[] a) {
        long[][][][] ans = new long[2 * n][n][1 << (2 * n)][4];

        for (int f = 0; f < 2; f++) {
            if ((a[0] & (1 << f)) != 0) {
                for (int s = 0; s < 2; s++) {
                    if (n == 1 && f == s)
                        continue;
                    if ((a[1] & (1 << s)) != 0) {
                        for (int i = 0; i < n; i++) {
                            for (int j = 0; j < n; j++) {
                                if (i == j)
                                    continue;
                                ans[1][j][(1 << (f * n + i)) + (1 << (s * n + j))][2 * f + s] = 1;
                            }
                        }
                    }
                }
            }
        }

        for (int i = 1; i < 2 * n - 1; i++) {
            for (int prev = 0; prev < n; prev++) {
                for (int mask = 0; mask < 1 << (2 * n); mask++) {
                    for (int mask2 = 0; mask2 < 4; mask2++) {
                        int closed = ((mask2 & 1) + (mask2 >> 1)) % 2 == 0 ? -1 : (mask2 >> 1);
                        ;
                        for (int nt = 0; nt < 2; nt++) {
                            if (closed == nt || (a[i + 1] & (1 << nt)) == 0)
                                continue;
                            for (int next = 0; next < n; next++) {
                                if ((mask & (1 << (nt * n + next))) != 0 || prev == next) {
                                    continue;
                                }
                                ans[i + 1][next][mask | (1 << (nt * n + next))][((mask2 & 1) << 1) + nt] += ans[i][prev][mask][mask2];
                                ans[i + 1][next][mask | (1 << (nt * n + next))][((mask2 & 1) << 1) + nt] %= mod;
                            }
                        }
                    }
                }
            }
        }

        long answer = 0;
        for (int prev = 0; prev < n; prev++)
            for (int mask = 0; mask < 4; mask++) {
                answer += ans[2 * n - 1][prev][(1 << (2 * n)) - 1][mask];
            }

        return answer;
    }

    public void run() {
        try {
            br = new BufferedReader(new InputStreamReader(System.in));
            out = new PrintWriter(System.out);

            int t = nextInt();
            for (int j = 0; j < t; j++) {
            	int n = nextInt();
            	int[] a = new int[2 * n];
            	for (int i = 0; i < 2 * n; i++) {
          		a[i] = nextInt();
            	}
 
          	  out.println(solve(n, a));
            }

/*            int pow = 9;
            for (int i = 1; i < 10; i++) {
                for (int value = 0; value < pow; value++) {
                    int[] a = new int[2 * i];
                    int x = value;
                    for (int j = 0; j < 2 * i; j++) {
                        a[j] = (x % 3) + 1;
                        x /= 3;
                    }
                    long a1 = solve(i, a);
                    long a2 = solve2(i, a);
                    if (a1 != a2) {
                        System.err.println(a1 + " " + a2);
                        return;
                    }            
                }
                pow *= 9;
                System.err.println("Finished checking " + i);
            }                    */

            out.close();         
        } catch (IOException e) {
            e.printStackTrace(); 
            System.exit(1);
        }
    }
}

