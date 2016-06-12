import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;
import java.util.Arrays;
import java.util.PrimitiveIterator;

public class Stones {

    final boolean onlineJudge = System.getProperty("ONLINE_JUDGE") != null;
    final StreamTokenizer tok = new StreamTokenizer(new InputStreamReader(System.in));

    final int[] test1 = new int[]{17, 3, 1, 4, 3};//2
    final int[] test2 = new int[]{15, 3, 1, 5, 11};//2
    final int[] test3 = new int[]{3, 3, 1, 2, 3};//1
    final PrimitiveIterator.OfInt iter = Arrays.stream(test1).iterator();

    int nextInt() throws IOException {
        if (!onlineJudge) {
            return iter.nextInt();
        } else {
            tok.nextToken();
            return (int) tok.nval;
        }
    }

    private void run() throws IOException {
        final int N = nextInt();
        final int M = nextInt();
        final int[] k = new int[M];
        for (int i = 0; i < M; ++i) {
            k[i] = nextInt();
        }
        Arrays.sort(k);

        final byte[] res = new byte[N + k[M - 1] + 1];
        Arrays.fill(res, (byte) 2);
        for (int current = k[0]; current <= N; ++current) {
            boolean canInvert = false;
            for (int move : k) {
                if (current > move) {
                    canInvert = res[current - move] == 2;
                }
                if (canInvert) {
                    break;
                }
            }
            if (canInvert) {
                res[current] = 1;
            }
        }
        System.out.println((int) res[N]);
    }

    public static void main(String[] args) throws IOException {
        new Stones().run();
    }
}
