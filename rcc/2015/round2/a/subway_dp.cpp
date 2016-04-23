#include <bits/stdc++.h>

int main() {
    int tests;
    scanf("%d", &tests);
    for (int test = 0; test != tests; ++test) {
        int a, b, k;
        scanf("%d%d%d", &a, &b, &k);
        if (k == 1) {
            puts(((a == b) || (a >= 99 && b >= 99)) ? "0" : "-1");
            continue;
        }
        if (a > b)
            std::swap(a, b);
        int days = 0;
        if (a > 99) {
            b -= a - 99;
            days += a - 99;
            a = 99;
        }
        bool foundAnswer = false;
        while (a > 0) {
            if (a * k == std::min(99, b)) {
                foundAnswer = true;
                break;
            }
            a--;
            b--;
            days++;
        }
        printf("%d\n", foundAnswer ? days : -1);
    }
    return 0;
}