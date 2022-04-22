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

const int MAXN = 5e3 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

int dp[MAXN] = {0};

void solve() {
    
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
	string s1, s2;
	int a, b, c;
	cin >> a >> b >> c >> s1 >> s2;

	int n = s1.size(), m = s2.size();
	for(int j = 1; j <= m; ++j)
		dp[j] = j * a;

	int diff;
	for(int i = 1; i <= n; ++i){
		int p = dp[0];
		dp[0] = i * b;
		for(int j = 1; j <= m; ++j){
			int tmp = dp[j];
			int diff = abs(s1[i - 1] - s2[j - 1]);
			dp[j] = min({dp[j] + b, dp[j - 1] + a, p + min(diff, 26 - diff) * c});
			p = tmp;
		}

	}
			
	cout << dp[m] << '\n';
}

