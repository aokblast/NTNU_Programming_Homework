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

const int MAXN = 2e5 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;



void solve() {
    
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
	int n, a[MAXN], tmp[MAXN], sort1[MAXN], sort2[MAXN];
	cin >> n;
	for(int i = 0; i < n; ++i){
		cin >> a[i];
		if(i % 2)
			sort2[i >> 1] = a[i];
		else 
			sort1[i >> 1] = a[i];
	}
	
	long cnt = 0;
	function<void(int, int, int *)> sort = [&](int l, int r, int *arr){
		if(l >= r)
			return;
		int mid = l + ((r - l) >> 1);
		sort(l, mid, arr);
		sort(mid + 1, r, arr);
		
		int idx = l, p1 = l, p2 = mid + 1;
		while(p1 <= mid && p2 <= r)
			tmp[idx++] = arr[p1] <= arr[p2] ? arr[p1++] : (cnt += (mid - p1 + 1), arr[p2++]);
		while(p1 <= mid)
			tmp[idx++] = arr[p1++];
		while(p2 <= r)
			tmp[idx++] = arr[p2++];
		for(int i = l; i <= r; ++i)
			arr[i] = tmp[i];
	};
	long res = 0;
	sort(0, (n + 1) / 2 - 1, sort1);
	res += cnt;
	cnt = 0;
	sort(0, n  / 2 - 1, sort2);
	
	res += cnt;
	sort(0, n - 1, a);
	bool ans = true;
	for(int i = 0; i < n; ++i){
		if(i % 2){
			if(a[i] != sort2[i >> 1]){
				ans = false;
				break;
			}
		}else{
			if(a[i] != sort1[i >> 1]){
				ans = false;
				break;
			}
		}


	}
	cout << (ans ? "yes" : "no") << '\n' << res << '\n';
	
}
