/**
 * Created by shovkoplyas on 25.05.2015.
 */
import ru.ifmo.testlib.Checker;
import ru.ifmo.testlib.InStream;
import ru.ifmo.testlib.Outcome;

public class Check implements Checker {

    @Override
    public Outcome test(InStream inf, InStream ouf, InStream ans) {
        int t = inf.nextInt();
        for (int i = 0; i < t; i++) {
            int cAns = ouf.nextInt();
            int jAns = ans.nextInt();
            if (cAns != jAns) {
                return new Outcome(Outcome.Type.WA,
                        String.format("In test #%d expected: %d numbers, found: %d", i + 1, jAns, cAns));
            }
            for (int j = 0; j < cAns; j++)
            {
                int cA = ouf.nextInt();
                int jA = ans.nextInt();
                if (cA != jA) {
                    return new Outcome(Outcome.Type.WA,
                            String.format("In test #%d in #%d pos expected: %d, found: %d", i + 1, j + 1, jA, cA));
                }
            }
        }
        return new Outcome(Outcome.Type.OK, String.format("%d tests", t));
    }
}
