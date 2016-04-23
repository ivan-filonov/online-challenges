import ru.ifmo.testlib.Checker;
import ru.ifmo.testlib.InStream;
import ru.ifmo.testlib.Outcome;
import static ru.ifmo.testlib.Outcome.Type.*;

public class Check implements Checker {
    public Outcome outcome(Outcome.Type type, final String format) {
        return new Outcome(type, format);
    }

    public Outcome test(InStream inf, InStream ouf, InStream ans) {
        int tests = inf.nextInt();
        for (int test = 0; test < tests; test++) {
            int n = inf.nextInt();
            String ja = ans.nextLine();
            String pa = ouf.nextLine();
            if (!ja.equals(pa)) {
                return outcome(WA, String.format("test %d: expected %s, found %s", test + 1, ja, pa));
            }
        }
        return outcome(OK, String.format("%d tests", tests));
    }
}
