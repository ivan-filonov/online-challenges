#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

void solve() {
	double ans = 0;
	int a1, a2, b1, b2, t;
	cin >> a1 >> b1 >> a2 >> b2 >> t;
	b1 *= -1, b2 *= -1;
	int cur1 = 0, cur2 = 0;
	for (int curt = 0; curt < t; curt++) {
		long long diff1 = cur1 - cur2;
		int dd = curt % 24;
		int c1 = 0, c2 = 0;
		if (dd < 12) {
			c1 = a1, c2 = a2;
		} else {
			c1 = b1, c2 = b2;
		} 
		cur1 += c1, cur2 += c2;
		long long diff2 = cur1 - cur2;
		if (diff1 <= 0 && diff2 <= 0) {
			continue;
		}
		if (diff1 >= 0 && diff2 >= 0) {
			ans += 1;
			continue;
		}
		if (diff1 > 0 && diff2 < 0) {
			double tt = diff1 * 1.0 / (c2 - c1);
			ans += tt;
			continue;
		}
		if (diff1 < 0 && diff2 > 0) {
			double tt = diff2 * 1.0 / (c1 - c2);
			ans += tt;
			continue;
		}
		assert(false);
	}
	assert(0 <= ans && ans <= t);
	cout.precision(10);
	cout << fixed << ans << "\n";
}

int main() {
	int tt;
	cin >> tt;
	while (tt--) solve();
}