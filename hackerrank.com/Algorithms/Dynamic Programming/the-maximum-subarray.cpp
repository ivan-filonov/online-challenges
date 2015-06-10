#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int ntests;
    cin >> ntests;
    for(int x = 0; x != ntests; ++x) {
        int c;
        cin >> c;
        int cmax = 0, nmax = 0, imax = ~0x7fffffff, cur = 0;
        for(int i = 0, j; i < c; ++i) {
            cin >> j;
            if( j > imax ) {
              imax = j;
            }
            if( j > 0 ) {
                nmax += j;
            }
            cur += j;
            if( cur < 0) {
                cur = 0;
            }
            if( cur > cmax ) {
                cmax = cur;
            }
        }
        if(imax > 0) {
          cout << cmax << ' ' << nmax << "\n";
        } else {
          cout << imax << ' ' << imax << "\n";
        }
    }
    return 0;
}

