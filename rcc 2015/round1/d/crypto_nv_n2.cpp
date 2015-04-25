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
	int m;
	cin >> m;
	for (int i = 0; i <= m; i++) {
    	int n = s.length();
        vector<long long> dp(n + 10);
        dp[0] = 1;
        for (int i = 0; i < n; i++) {
        	if (dp[i] == 0) continue;
        	dp[i] %= MOD;
        	for (int x = 0; x < 10; x++) 
    			if (s.substr(i, len[x]) == dig[x]) {
            		dp[i + len[x]] += dp[i];
        		}
        }
        cout << dp[n] % MOD << "\n";
        int pos, alp;
        cin >> pos >> alp;
        s[pos - 1] = (char)('0' + alp);
    }
    return 0;
}
