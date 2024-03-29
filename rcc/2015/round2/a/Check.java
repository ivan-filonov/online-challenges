import ru.ifmo.testlib.Checker;
import ru.ifmo.testlib.InStream;
import ru.ifmo.testlib.Outcome;
import static ru.ifmo.testlib.Outcome.Type.*;

public class Check implements Checker {
    public Outcome outcome(Outcome.Type type, final String format, final Object... args) {
        return new Outcome(type, String.format(format, args));
    }

    public Outcome test(InStream inf, InStream ouf, InStream ans) {
        int testsCount = inf.nextInt();
        for (int test = 0; test < testsCount; test++) {
            int x = inf.nextInt();
            int y = inf.nextInt();
            int k = inf.nextInt();
            String pAns = ouf.nextToken();
            String jAns = ans.nextToken();
            if (!pAns.equals(jAns)) {
                return outcome(WA, String.format("%d-th answer is wrong - expected %s, found %s, test = [%d, %d, %d]", test + 1, jAns, pAns, x, y, k));
            }
        }
        return outcome(OK, String.format("Correct answer, %d tests", testsCount));
    }
}
