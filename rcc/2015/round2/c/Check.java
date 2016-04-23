/**
 * Created by shovkoplyas on 19.04.2015.
 */
import ru.ifmo.testlib.Checker;
import ru.ifmo.testlib.InStream;
import ru.ifmo.testlib.Outcome;

public class Check implements Checker {

    final double EPS = 0.000002;

    @Override
    public Outcome test(InStream inf, InStream ouf, InStream ans) {
        int t = inf.nextInt();
        for (int i = 0; i < t; i++) {
            double cAns = ouf.nextDouble();
            double jAns = ans.nextDouble();
            if (Math.abs(cAns - jAns) > EPS) {
                return new Outcome(Outcome.Type.WA,
                        String.format("In string #%d expected: %f, found: %f", i + 1, jAns, cAns));
            }
        }
        return new Outcome(Outcome.Type.OK, String.format("%d tests", t));
    }
}
