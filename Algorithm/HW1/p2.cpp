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

const int MAXN = 1e5 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

struct E{
	int a, b;
	bool operator<(const E& e) const{
		return a == e.a ? b < e.b : a < e.a;
	}
};
void solve() {
    
}


int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
	int n;
	cin >> n;
	E sorted[MAXN];
	for(int i = 0; i < n; ++i)
		cin >> sorted[i].a >> sorted[i].b;
	sort(sorted, sorted + n);
	int res = 1;
	int x = sorted[n - 1].a;
	int y = sorted[n - 1].b;
	for(int i = n - 2; i >= 0; --i){
		res += sorted[i].a == x || sorted[i].b >= y;
		y = max(sorted[i].b, y);

	}
	
	cout << res << '\n';

}

