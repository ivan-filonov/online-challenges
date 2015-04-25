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
        bool foundAnswer = false;
        int days = 0;
        while (true) {
            if (std::min(a, 99) * k == std::min(b, 99)) {
                printf("%d\n", days);
                foundAnswer = true;
            }
            --a;
            --b;
            ++days;
            if (a == 0) {
                break;
            }
        }
        if (!foundAnswer) {
            printf("-1\n");
        }
    }
    return 0;
}