#include <bits/stdc++.h>

void readNumber(std::string &s) {
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        int digit;
        std::cin >> digit;
        s += digit + '0';
    }
}

int main() {
    #ifndef ONLINE_JUDGE
        freopen("notation.in", "r", stdin);
        freopen("notation.out", "w", stdout);
    #endif
    int testsCount;
    std::cin >> testsCount;
    while (testsCount--) {
        std::string x, y, z;
        readNumber(x);
        readNumber(y);
        readNumber(z);
        std::vector <int> multiply(x.size() + y.size(), 0);
        std::reverse(x.begin(), x.end());
        std::reverse(y.begin(), y.end());
        for (size_t i = 0; i != x.size(); ++i) {
            for (size_t j = 0; j != y.size(); ++j) {
                multiply[i + j] += (x[i] - '0') * (y[j] - '0');
            }
        }
        while (multiply.size() > 1 && multiply.back() == 0) {
            multiply.pop_back();
        }
        std::string result = "";
        for (int i = (int)multiply.size() - 1; i >= 0; --i) {
            int x = multiply[i];
            std::string digits;
            while (x > 0) { 
                digits += (x % 10) + '0';
                x /= 10;
            }
            std::reverse(digits.begin(), digits.end());
            result += digits;
        }
        std::cout << result << '\n';
        printf("%s\n", result == z ? "Infinity" : "Finite");
    }
    return 0;
}
