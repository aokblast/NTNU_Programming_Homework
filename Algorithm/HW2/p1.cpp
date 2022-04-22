#include <bits/stdc++.h>

using namespace std;

template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '(' << p.first << ", " << p.second << ')'; }
template<typename T_container, typename T = typename enable_if<!is_same<T_container, string>::value, typename T_container::value_type>::type> ostream& operator<<(ostream &os, const T_container &v) { os << '{'; string sep; for (const T &x : v) os << sep << x, sep = ", "; return os << '}'; }
void dbg_out() { cerr << endl; }
template<typename Head, typename... Tail> void dbg_out(Head H, Tail... T) { cerr << ' ' << H; dbg_out(T...); }
#ifdef LOCAL
#define dbg(...) cerr << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#else
#define dbg(...)
#endif

#define ar array
#define ll long long
#define ld long double
#define sza(x) ((int)x.size())
#define all(a) (a).begin(), (a).end()

const int MAXN = 3e3 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

int dp[MAXN] = {0};

void solve() {
    
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

	int n, u, w, v, a, cv, cw;
	cin >> n >> u;


	while(n--){
		cin >> w >> v >> a;

		for(int i = 1; i <= a; i <<= 1){
			a -= i;
			cv = v * i;
			cw = w * i;
			for(int j = u; j >= cw; --j)
				dp[j] = max(dp[j], dp[j - cw] + cv);

		}
		if(a){
			cw = w * a;
			cv = v * a;
			for(int j = u; j >= cw; --j)
				dp[j] = max(dp[j], dp[j - cw] + cv);
		}	
	}
	cout << dp[u] << '\n';
}

