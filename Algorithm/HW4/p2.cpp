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
const int MAXM = 5e5 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

int djs[MAXN];

struct E{
  int u, v, d;

  bool operator<(const E &e) const {
    return d > e.d;
  }
}edges[MAXM];

void solve() {
    
}



int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    int n, m;

    cin >> n >> m;

    for(int i = 1; i <= n; ++i)
      djs[i] = i;

    for(int i = 0; i < m; ++i)
      cin >> edges[i].u >> edges[i].v >> edges[i].d;

    sort(edges, edges + m);

    function<int(int)> parent = [&](int u){
      return u == djs[u] ? u : djs[u] = parent(djs[u]);
    };

    int res = 0,  g = n;
    int i;
    for(i = 0; i < m; ++i){
      int p1 = parent(edges[i].u), p2 = parent(edges[i].v);
      if(p1 == p2)
        continue;
      djs[p1] = p2;
      if((--g) == 1)
        break;
    }
    g = n;

    for(int i = 1; i <= n; ++i)
      djs[i] = i;

    while(i < (m - 1) && edges[i].d == edges[i + 1].d)
      ++i;

    for(; i >= 0; --i){
      int p1 = parent(edges[i].u), p2 = parent(edges[i].v);
      if(p1 == p2)
        continue;
      djs[p1] = p2;
      res += edges[i].d;
      if((--g) == 1)
        break;
    }
    

    cout << res << '\n';
    

}

