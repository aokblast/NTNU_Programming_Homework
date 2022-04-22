#include <bits/stdc++.h>

using namespace std;

template<typename A, typename B> ostream& operator<<(ostream& os, const pair<A, B>& p) { return os << '(' << p.first << ", " << p.second << ')'; }
template<typename T_container, typename T = typename enable_if<!is_same<T_container, string>::value, typename T_container::value_type>::type> ostream& operator<<(ostream& os, const T_container& v) { os << '{'; string sep; for (const T& x : v) os << sep << x, sep = ", "; return os << '}'; }
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

const int MAXN = 2e5 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

long tree[MAXN << 2] = { 0 };
long arr[MAXN] = { 0 };

void insert(int p, int val, int u = 1, int pl = 0, int pr = 2e5) {
	if (pl == pr) {
		tree[u] = val;
		return;
	}
	int mid = (pl + pr) >> 1;
	if (mid >= p)
		insert(p, val, u << 1, pl, mid);
	else
		insert(p, val, u << 1 | 1, mid + 1, pr);

	tree[u] = arr[tree[u << 1]] < arr[tree[u << 1 | 1]] ? tree[u << 1] : tree[u << 1 | 1];

}


int query(int l, int r, int u = 1, int pl = 0, int pr = 2e5) {
	if (l <= pl && r >= pr)
		return tree[u];
	int mid = (pl + pr) >> 1;
	int val = tree[u], a = -1, b = -1;
	if (mid >= l)
		a = query(l, r, u << 1, pl, mid);
	if (r >= mid + 1)
		b = query(l, r, u << 1 | 1, mid + 1, pr);


	if (a == -1) {
		return b;
	}
	else if (b == -1) {
		return a;
	}
	else {
		return arr[a] < arr[b] ? a : b;
	}

}

void solve() {

}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0); cout.tie(0);
	int n;
	cin >> n;
	const long MOD = 1e9 + 7;
	for (int i = 0; i < n; ++i)
		cin >> arr[i];
	for (int i = 0; i < n; ++i)
		insert(i, i, 1, 0, n - 1);
	long res = 0;
	for (int i = 0; i < n - 1; ++i) {
		int ans = query(i + 1, n - 1, 1, 0, n - 1);
		if (arr[i] > arr[ans] && i < ans)
			res = (res + (arr[i] + arr[ans]) * (ans - i)) % MOD, swap(arr[i], arr[ans]), insert(ans, ans, 1, 0, n - 1);

	}
	cout << res << '\n';

}


