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

const int MAXN = 3e5 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
	int n, d;	
	cin >> n >> d;
	vector<int> adjs[n + 1];
	int dis[n + 1];
	int f, t;
	
	for(int i = 0; i < n - 1; ++i){
		cin >> f >> t;
		adjs[f].push_back(t);
		adjs[t].push_back(f);
	}
	int last = 0;

	memset(dis, 0, sizeof(dis));	
   	function<void(int, int)> dfs1 = [&](int u, int p){
		for(const int adj : adjs[u]){
			if(adj == p)
				continue;
			dis[adj] = dis[u] + 1;
			if(dis[adj] > dis[last])
				last = adj;
			dfs1(adj, u);
		}
	};
	int top = 0;
	int st[n + 1];

	for(int i = 1; i <= n; ++i)
		sort(adjs[i].begin(), adjs[i].end());

	function<bool(int, int, int)> dfs2 = [&](int u, int p, int layer){
		if(layer > d)
			return false;
		st[top++] = u;
		for(const auto adj : adjs[u]){
			if(adj == p)
				continue;
			if(!dfs2(adj, u, layer + 1))
				return false;
		}
		return true;
	};
	dis[1] = 0;
	dfs1(1, 0);
	dis[last] = 0, dfs1(last, 0);
	if(dis[last] > d * 2 + 2){
		cout << -1 << '\n';
		return 0;
	}
	for(int i = 1; i <= n; ++i){
		top = 0;
		if(dfs2(i, 0, 0)){
			for(int j = 0; j < n; ++j)
				cout << st[j] << ' ';
			cout << '\n';
			return 0;
		}
	}

}
