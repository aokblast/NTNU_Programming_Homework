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



void solve() {

}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
	int t, n;
	cin >> n >> t;

	int nums[n];
	for(int i = 0; i < n; ++i)
		cin >> nums[i];
	using uil = unordered_map<int, long>;
	function<uil(int, int)> dac = [&](int l, int r){
		if(l >= r){
			uil m;
			m[nums[l]] = 1;
			return m;
		}
		int mid = l + (r - l) / 2;
		auto a = dac(l, mid);
		auto b = dac(mid + 1, r);
		uil res;
		for(const auto [sum1, c1] : a){
			res[sum1] += c1;
			for(const auto [sum2, c2] : b){
				res[sum1 + sum2] += c1 * c2;
			}
		}
		for(const auto [sum, c] : b)
			res[sum] += c;
		return res;

	};
	
	auto l = dac(0, n / 2 - 1);
	auto r = dac(n / 2, n - 1);
	long res = 0;
	for(const auto [sum, c] : l)
		if(r.find(t - sum) != r.end())
			res += c * r[t - sum];

	
	cout << (res + l[t] + r[t]) << '\n';

	
	



}
