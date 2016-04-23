import ru.ifmo.testlib.Checker;
import ru.ifmo.testlib.InStream;
import ru.ifmo.testlib.Outcome;
import static ru.ifmo.testlib.Outcome.Type.*;

public class Check implements Checker {
    public Outcome outcome(Outcome.Type type, final String format) {
        return new Outcome(type, format);
    }

    public Outcome test(InStream inf, InStream ouf, InStream ans) {
	int n = inf.nextInt();
	int m = inf.nextInt();
        int ja = ans.nextInt();
        int pa = ouf.nextInt();
        if (ja != pa) {
            return outcome(WA, String.format("expected %d, found %d", ja, pa));
        }
        return outcome(OK, String.format("n = %d, m = %d, ans = %d", n, m, ja));
    }
}
