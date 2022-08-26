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

const int MAXN = 1e3 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

int arr[2][MAXN] = {{0}};

void solve() {
    
}



int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
	string line;
	getline(cin, line);

	int n = line.size();

	for(int i = 0; i < n; ++i){
		arr[0][i] = line[i] - '0';
	}	

	getline(cin, line);

	for(int i = 0; i < n; ++i)
		arr[1][i] = line[i] - '0';
	
	int res = 0;

	for(int i = 0; i < n - 1; ++i){
		if(arr[0][i] && arr[1][i]){
			continue;
		}else if(!arr[0][i] && !arr[1][i]){
			if(!arr[0][i + 1] || !arr[1][i + 1]){
				int idx = arr[0][i + 1] != 0;
				arr[0][i] = arr[1][i] = arr[idx][i + 1] = 1, ++res;
			}
		}else{
			if(!arr[0][i + 1] && !arr[1][i + 1])
				arr[0][i] = arr[1][i] = arr[0][i + 1] = arr[1][i + 1] = 1, ++res;
		}
	}
	cout << res << '\n';


}

