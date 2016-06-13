
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;
import java.util.Arrays;
import java.util.PrimitiveIterator;

public class Berries {

    final boolean onlineJudge = System.getProperty("ONLINE_JUDGE") != null;
    final StreamTokenizer tok = new StreamTokenizer(new InputStreamReader(System.in));

    final int[] test1 = new int[]{1, 2, 2, 1, 0, 3};//1 1
    final PrimitiveIterator.OfInt iter = Arrays.stream(test1).iterator();

    int nextInt() throws IOException {
        if (!onlineJudge) {
            return iter.nextInt();
        } else {
            tok.nextToken();
            return (int) tok.nval;
        }
    }

    void run() throws IOException {
        //A+a B+b
        final int a1 = nextInt();
        final int b1 = nextInt();
        //A+a+b B
        final int a2 = nextInt();
        final int b2 = nextInt();
        //A B+a+b
        final int a3 = nextInt();
        final int b3 = nextInt();
        System.out.println(Integer.toString(a1 - a3) + " " + Integer.toString(b1 - b2));
    }

    public static void main(String[] args) throws IOException {
        new Berries().run();
    }
}
