import ru.ifmo.testlib.Checker;
import ru.ifmo.testlib.InStream;
import ru.ifmo.testlib.Outcome;
import ru.ifmo.testlib.Outcome.Type;
import java.util.BitSet;

public class Check implements Checker {

    @Override
    public Outcome test(InStream inf, InStream ouf, InStream ans) {
        int n = inf.nextInt();
        int m = inf.nextInt();
        BitSet state = new BitSet(n);
        String s = inf.nextToken();
        for (int i = 0; i < n; i++) {
            state.set(i, s.charAt(i) == '1');
        }
        int[] type = new int[m];
        BitSet[] bs = new BitSet[m];
        for (int i = 0; i < m; i++) {
            type[i] = inf.nextInt();
            bs[i] = new BitSet(n);
            s = inf.nextToken();
            for (int j = 0; j < n; j++) {
                bs[i].set(j , s.charAt(j) == '1');
            }
        }

        String rouf = ouf.nextToken();
        String rans = ans.nextToken();

        if (!rouf.equals("NO") && !rans.equals(rouf)) {
            return new Outcome(Type.WA, "The answer of the participant isn't equal to the answer of jury");
        }

        if (rouf.equals("NO")) {
            return new Outcome(Type.OK, "OK");
        }

        int k = ouf.nextInt();

        if (k > 500) {
            return new Outcome(Type.WA, "The number of turns is more than expected");
        }


        BitSet now = new BitSet(n);
        for (int i = 0; i < k; i++) {
            int id = ouf.nextInt() - 1;
            //);
            if (type[id] == 1) {
                now.or(bs[id]);
            } else {
                now.andNot(bs[id]);
            }
        }

        for (int i = 0; i < n; i++) {
            if (now.get(i) != state.get(i)) {
                return new Outcome(Type.WA, "The result state is not equal to the requested one");
            }
        }

        if (rans.equals("NO")) {
            return new Outcome(Type.FAIL, "The participant has found answer while jury doesn't");
        }

        return new Outcome(Type.OK, "OK");
    }
}
