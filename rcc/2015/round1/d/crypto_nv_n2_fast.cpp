#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

const int MOD = 1e9 + 7;

string to_string(int i) {
    stringstream ss;
    ss << i;
    string s;
    ss >> s;
    return s;
}


int main() {
	int a, b, c;
	cin >> a >> b >> c;
	vector<string> dig(10);
	vector<int> len(10);
	for (int x = 0; x < 10; x++) {
		dig[x] = to_string(a * x * x + b * x + c);
		len[x] = dig[x].length();
		// cerr << dig[x] << " " << len[x] << endl;
	}
    
    string s;
	cin >> s;
   	int n = s.length();
    vector<long long> dp(n + 20);
    dp[0] = 1;

	int m, last = 0;
	cin >> m;

	for (int i = 0; i <= m; i++) {
		last = max(last - 5, 1);
		for (int i = last; i <= n + 2; i++) 
        	dp[i] = 0;
        for (int i = max(last - 5, 0); i < last; i++) {
        	if (dp[i] == 0) continue;
        	dp[i] %= MOD;
        	for (int x = 0; x < 10; x++) 
    			if (s.substr(i, len[x]) == dig[x]) {
            		dp[i + len[x]] += dp[i];
        		}
        }
        for (int i = last; i < n; i++) {
        	if (dp[i] == 0) continue;
        	dp[i] %= MOD;
        	for (int x = 0; x < 10; x++) 
    			if (s.substr(i, len[x]) == dig[x]) {
            		if (i + len[x] >= last) dp[i + len[x]] += dp[i];
        		}
        }
        cout << dp[n] % MOD << "\n";
        int pos, alp;
        cin >> pos >> alp;
        s[pos - 1] = (char)('0' + alp);
        last = pos - 1;
    }
    
    return 0;
}
