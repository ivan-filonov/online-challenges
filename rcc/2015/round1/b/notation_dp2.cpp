#include <bits/stdc++.h>

int main() {
    #ifndef ONLINE_JUDGE
        freopen("notation.in", "r", stdin);
        freopen("notation.out", "w", stdout);
    #endif
    int testsCount;
    std::cin >> testsCount;
    while (testsCount--) {
        std::string x, y, z;
        std::cin >> x >> y >> z;
        std::vector <int> multiply(x.size() + y.size(), 0);
        std::reverse(x.begin(), x.end());
        std::reverse(y.begin(), y.end());
        bool isInfinity = true;
        for (size_t i = 0; i != x.size(); ++i) {
            for (size_t j = 0; j != y.size(); ++j) {
                multiply[i + j] += (x[i] - '0') * (y[j] - '0');
                if (multiply[i + j] >= 10) {
                    isInfinity = false;
                }
            }
        }
        while (multiply.size() > 0 && multiply.back() == 0) 
            multiply.pop_back();
        std::reverse(z.begin(), z.end());
        isInfinity &= (z.size() == multiply.size());
        for (size_t i = 0; i != z.size() && isInfinity; ++i)
            isInfinity &= (z[i] == multiply[i] + '0');
        printf("%s\n", isInfinity ? "Infinity" : "Finite");
    }
    return 0;
}
