import java.util.Arrays;

/**
 *
 * @author ifilonov
 */
public class Hungarian {

    public static void main(String[] args) {
        int[][] test1 = new int[][]{
            {0, 0, 0, 0},
            {0, -3, -2, -1},
            {0, -2, -2, -2},
            {0, -1, -2, -1}};
        final Hungarian solver1 = new Hungarian();
        solver1.process(test1);
    }

    private void process(int[][] weights) {
        final int num_lines = weights.length - 1;
        final int num_columns = weights[0].length - 1;//assume all lines are the same length!!!
        final int INF = 1 << 30;

        final int[] line_level = new int[num_lines + 1];
        final int[] column_level = new int[num_columns + 1];
        final int[] assigned_line_for_column = new int[num_columns + 1];
        final int[] way = new int[num_columns + 1];

        for (int line_to_add = 1; line_to_add <= num_lines; ++line_to_add) {
            assigned_line_for_column[0] = line_to_add;
            int cur_column = 0;

            final int[] minv = new int[num_columns + 1];
            final boolean[] used = new boolean[num_columns + 1];

            Arrays.fill(minv, INF);
            do {
                used[cur_column] = true;
                final int cur_line = assigned_line_for_column[cur_column];
                int delta = INF;
                int next_column = 0;

                for (int column = 1; column <= num_columns; ++column) {
                    if (used[column]) {
                        continue;
                    }
                    final int cur = weights[cur_line][column] - line_level[cur_line] - column_level[column];
                    if (cur < minv[column]) {
                        minv[column] = cur;
                        way[column] = cur_column;
                    }
                    if (minv[column] < delta) {
                        delta = minv[column];
                        next_column = column;
                    }
                }

                for (int column = 0; column <= num_columns; ++column) {
                    if (used[column]) {
                        line_level[assigned_line_for_column[column]] += delta;
                        column_level[column] -= delta;
                    } else {
                        minv[column] -= delta;
                    }
                }
                cur_column = next_column;
            } while (0 != assigned_line_for_column[cur_column]);

            do {
                final int prev_column = way[cur_column];
                assigned_line_for_column[cur_column] = assigned_line_for_column[prev_column];
                cur_column = prev_column;
            } while (0 != cur_column);
        }

        final int[] ans = new int[num_columns + 1];
        for (int column = 1; column <= num_columns; ++column) {
            ans[assigned_line_for_column[column]] = column;
        }
        System.out.println("answer: " + Arrays.toString(ans));
    }
}

