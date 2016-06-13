
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;
import java.util.Arrays;
import java.util.PrimitiveIterator;

public class Turn {

    final boolean onlineJudge = System.getProperty("ONLINE_JUDGE") != null;
    final StreamTokenizer tok = new StreamTokenizer(new InputStreamReader(System.in));

    final int[] test1 = new int[]{5, 3, 6, 7, 2};//0
    final int[] test2 = new int[]{5, 3, 20, 0, 0};//5
    final PrimitiveIterator.OfInt iter = Arrays.stream(test2).iterator();

    int nextInt() throws IOException {
        if (!onlineJudge) {
            return iter.nextInt();
        } else {
            tok.nextToken();
            return (int) tok.nval;
        }
    }
    
    private void run() throws IOException {
        final int k = nextInt();
        final int n = nextInt();
        
        int state = 0;
        for(int i = 0; i < n;++i) {
            state += nextInt();
            state -= Math.min(state, k);
        }
        
        System.out.println(state);
    }

    public static void main(String[] args) throws IOException {
        new Turn().run();
    }
}
