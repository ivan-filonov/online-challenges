
#include<stdio.h>
#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<memory.h>
#include<map>
#include<set>
#include<queue>
#include<list>
#include<sstream>
#include<cstring>
#define mp make_pair
#define pb push_back      
#define F first
#define S second
#define SS stringstream
#define sqr(x) ((x)*(x))
#define m0(x) memset(x,0,sizeof(x))
#define m1(x) memset(x,63,sizeof(x))
#define CC(x) cout << (x) << endl
#define pw(x) (1ll<<(x))
#define buli(x) __builtin_popcountll(x)
#define M 1000000007
#define N 211111
 
#define TASK "notation"
 
using namespace std;
typedef pair<int,int> pt;
 
int q1[N], q2[N], q3[N], qr[N];
 
int main(){
  freopen(TASK".in","r",stdin);   
  freopen(TASK".out","w",stdout);
    ios::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) {
        string a, b, c;
        cin >> a >> b >> c;
 
        int n = a.size();
        if (b.size() > n) n = b.size();
        if (c.size() > n) n = c.size();
        n *= 2;
 
        for (int i = 0; i < n; i++) {
            q1[i] = 0;
            q2[i] = 0;
            q3[i] = 0;
            qr[i] = 0;
        }
        for (int i = 0; i < a.size(); i++) q1[a.size() - 1 - i] = a[i] - '0';
        for (int i = 0; i < b.size(); i++) q2[b.size() - 1 - i] = b[i] - '0';
        for (int i = 0; i < c.size(); i++) q3[c.size() - 1 - i] = c[i] - '0';
 
        for (int i = 0; i < a.size(); i++) for (int j = 0; j < b.size(); j++) qr[i + j] += q1[i] * q2[j];
 
//        for (int i = 0; i < n; i++) cerr << qr[i] << " ";
//        cerr << endl;
 
        int bad = 0;
        for (int i = 0; i < n; i++) if (q3[i] != qr[i]) bad = 1;
        puts(bad ? "Finite" : "Infinity");
    }
    return 0;
}
