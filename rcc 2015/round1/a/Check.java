/**
 * Created by shovkoplyas on 18.03.2015.
 */
import ru.ifmo.testlib.Checker;
import ru.ifmo.testlib.InStream;
import ru.ifmo.testlib.Outcome;

public class Check implements Checker {

    @Override
    public Outcome test(InStream inf, InStream ouf, InStream ans) {
        int t = inf.nextInt();
        for (int i = 0; i < t; i++) {
            String cAns = ouf.nextLine();
            String jAns = ans.nextLine();
            if (!cAns.equals(jAns)) {
                return new Outcome(Outcome.Type.WA,
                        String.format("In string #%d expected: %s, found: %s", i + 1, jAns, cAns));
            }
        }
        return new Outcome(Outcome.Type.OK, String.format("%d tests", t));
    }
}
