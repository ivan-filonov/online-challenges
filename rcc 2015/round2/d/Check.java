import ru.ifmo.testlib.Checker;
import ru.ifmo.testlib.InStream;
import ru.ifmo.testlib.Outcome;
import ru.ifmo.testlib.Outcome.Type;

public class Check implements Checker {

	@Override
	public Outcome test(InStream inf, InStream ouf, InStream ans) {
	        int T = inf.nextInt();
        	for (int i = 0; i < T; i++) {
			long partAns = ouf.nextLong(), juryAns = ans.nextLong();
			if (partAns != juryAns) {
				return new Outcome(Type.WA, "Wrong answer on test case #" + (i + 1) + ", expected: " + juryAns + ", found: " + partAns);
			}
		}
		return new Outcome(Type.OK, "OK");
	}
}
