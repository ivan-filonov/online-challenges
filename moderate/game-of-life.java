//package my.fj.convay;
/* Sample code to read in test cases:
 import java.io.*;
 public class Main {
 public static void main (String[] args) throws IOException {
 File file = new File(args[0]);
 BufferedReader buffer = new BufferedReader(new FileReader(file));
 String line;
 while ((line = buffer.readLine()) != null) {
 line = line.trim();
 // Process line of input Here
 }
 }
 }
 */

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class Main {

    public static void main(String[] args) throws FileNotFoundException, IOException {
//        test1();
        codeEval(args);
    }

    private static void codeEval(String[] args) throws IOException {
        ArrayList<char[]> matrix = new ArrayList<>();
        readCells(args, matrix);
//        testCells(matrix);
        for (int i = 0; i < 10; ++i) {
            step(matrix);
        }
        for (char[] l : matrix) {
            System.out.println(new String(l));
        }
    }

    static void test1() {
        ArrayList<char[]> matrix = new ArrayList<>();
        matrix.add("**.".toCharArray());
        matrix.add("*..".toCharArray());
        matrix.add("...".toCharArray());
        System.out.println("ncount(0,0): " + ncount(matrix, 0, 0) + ", expected: 2");
        System.out.println("ncount(1,1): " + ncount(matrix, 1, 1) + ", expected: 3");
        System.out.println("ncount(2,0): " + ncount(matrix, 2, 0) + ", expected: 1");
        step(matrix);
        final String res = new String(matrix.get(0))
                + new String(matrix.get(1))
                + new String(matrix.get(2));
        System.out.println("res = " + res);
        System.out.println("exp = **.**....");
    }

    private static void testCells(ArrayList<char[]> matrix) {
        matrix.add(".........*".toCharArray());
        matrix.add(".*.*...*..".toCharArray());
        matrix.add("..........".toCharArray());
        matrix.add("..*.*....*".toCharArray());
        matrix.add(".*..*...*.".toCharArray());
        matrix.add(".........*".toCharArray());
        matrix.add("..........".toCharArray());
        matrix.add(".....*..*.".toCharArray());
        matrix.add(".*....*...".toCharArray());
        matrix.add(".....**...".toCharArray());
    }

    private static void readCells(String[] args, ArrayList<char[]> matrix) throws FileNotFoundException, IOException {
        File file = new File(args[0]);
        BufferedReader buffer = new BufferedReader(new FileReader(file));
        String line;
        while ((line = buffer.readLine()) != null) {
            line = line.trim();
            // Process line of input Here
            matrix.add(line.toCharArray());
        }
    }

    static boolean test(ArrayList<char[]> matrix, int r, int c) {
        if (r < 0 || r >= matrix.size()) {
            return false;
        }
        final char[] row = matrix.get(r);
        if (c < 0 || c >= row.length) {
            return false;
        }
        return '*' == row[c];
    }

    static int ncount(ArrayList<char[]> matrix, int r, int c) {
        int sum = 0;
        for (int dr = r - 1; dr <= r + 1; ++dr) {
            for (int dc = c - 1; dc <= c + 1; ++dc) {
                if (r == dr && c == dc) {
                    continue;
                }
                if (test(matrix, dr, dc)) {
                    ++sum;
                }
            }
        }
        return sum;
    }

    private static void step(ArrayList<char[]> matrix) {
        ArrayList<char[]> temp = new ArrayList<>();
        for (char[] line : matrix) {
            temp.add(new char[line.length]);
        }
        for (int r = 0; r < matrix.size(); ++r) {
            final char[] row = matrix.get(r);
            final char[] trow = temp.get(r);            
            for (int c = 0; c < row.length; ++c) {
                final int nc = ncount(matrix, r, c);
                char cv = row[c];
                if (nc < 2) {
                    cv = '.';
                } else if (nc == 3) {
                    cv = '*';
                } else if (nc > 3) {
                    cv = '.';
                }
                trow[c] = cv;
            }
        }
        matrix.clear();
        matrix.addAll(temp);
    }
}
