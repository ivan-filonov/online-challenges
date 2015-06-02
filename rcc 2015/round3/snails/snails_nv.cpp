#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void solve() {
	double ans = 0;
	int a1, a2, b1, b2, t;
	cin >> a1 >> b1 >> a2 >> b2 >> t;
	b1 *= -1, b2 *= -1;
	int cur1 = 0, cur2 = 0;
	while (t > 0) {
		int curt = min(12, t);
		// cerr << curt << "eeee\n";
		long long diff1 = cur1 - cur2;
		cur1 += a1 * curt;
		cur2 += a2 * curt;

		long long diff2 = cur1 - cur2;
		// cerr << "lololo\n";
		// cerr << cur1 << " " << cur2 << endl;
		// cerr << diff1 << " " << diff2 << endl;
		if ((diff1 * diff2 >= 0) && (diff1 + diff2 > 0)) ans += curt;
		if (diff1 * diff2 < 0) {
        	double tt = diff1 * 1.0 / abs(a2 - a1);
        //	cerr << tt << endl;
        	if (diff1 > 0) ans += tt;
        	else ans += (curt + tt);				
		}
		swap(a1, b1);
		swap(a2, b2);
		t -= 12;
		//cerr << "ans: " << ans << endl;
	}
	cout.precision(10);
	cout << fixed << ans << "\n";
}

int main() {
	int tt;
	cin >> tt;
	while (tt--) solve();
}