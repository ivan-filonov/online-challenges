#include <bits/stdc++.h>

int main() {
    int tests;
    scanf("%d", &tests);
    while (tests--) {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        int needOne = c & 1;
        puts((a >= needOne && c - a <= 2 * b) ? "YES" : "NO");
    }
    return 0;
}