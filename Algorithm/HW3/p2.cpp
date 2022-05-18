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


struct E{
	int x, y;
};

char g[MAXN][MAXN];
int stat[MAXN][MAXN];
bool vis[MAXN][MAXN];
E stk[MAXN * MAXN >> 3];
int n, m, sx, sy, tx, ty;

const int dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};


inline bool check(int x, int y){
	return x >= 0 && y >= 0 && x < n && y < m;
}

bool dfs(int lim){
	int top = 0;
	stk[top++] = {sx, sy};
	memset(vis, 0,  MAXN * n);
	vis[sx][sy] = true;
	while(top){
		auto [x, y] = stk[--top];
		for(int i = 0; i < 4; ++i){
			int xx = x + dir[i][0], yy = y + dir[i][1];
			if(!check(xx, yy) || stat[xx][yy] == -1 || stat[xx][yy] <= lim || vis[xx][yy])
				continue;
			if(xx == tx && yy == ty)
				return true;
			vis[xx][yy] = true;
			stk[top++] = {xx, yy};
			
		}
			
	
	}
	return false;

}


int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
	
	cin >> n >> m;

	queue<pair<int, int>> q;
	memset(stat, -1, sizeof(stat));
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < m; ++j){
			cin >> g[i][j];
			switch(g[i][j]){
				case 'L':
					q.push({i, j}), stat[i][j] = 0;
					break;
				case 'B':
					sx = i, sy = j, stat[i][j] = INT_MAX;
					break;
				case 'D':
					tx = i, ty = j, stat[i][j] = INT_MAX;
					break;
			}
		}
	}	
	int cur = 0;

	int l = 1, r = 0;

	while(!q.empty()){
		int sz = q.size();
		++cur;
		while(sz--){
			auto [x, y] = q.front(); q.pop();
			for(int i = 0; i < 4; ++i){
				int xx = x + dir[i][0], yy = y + dir[i][1];
				if(check(xx, yy) && g[xx][yy] == 'C' && stat[xx][yy] == -1)
					q.push({xx, yy}), stat[xx][yy] = cur, r = max(r, stat[xx][yy]);
			}
		}
	}

	while(l < r){
		int mid = l + (r - l) / 2;
		if(dfs(mid))
			l = mid + 1;
		else
			r = mid;
	}

	cout << ((l == 1) ? -1 : (l - 1)) << '\n';


}




