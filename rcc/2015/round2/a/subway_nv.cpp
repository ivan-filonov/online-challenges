//#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

long long solve() {
    long long a, b, k;
    in >> a >> b >> k;
    if (b > a) swap(a, b);
    if (k == 1) {
        if (min(a, b) > 98 || a == b) { 
            return 0;
        }
        else {
            return -1;
        }
    }
    if ((k * b - a) % (k - 1) == 0) {
        long long x = (k * b - a) / (k - 1);
        if (x > -1 && (a - x) < 100 && a != x) {  
            return x;
        }
    }
    if (99 % k == 0) {
        long long x = b - 99 / k;
        if (x > -1 && (a - x) > 98) {
            return x;
        }
    }
    return -1;
}

int main() {

    int t;
    cin >> t;
    while (t--) cout << solve() << "\n";
}

