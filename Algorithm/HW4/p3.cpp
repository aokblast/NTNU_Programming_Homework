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
const int MAXM = 3e5 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

using pii = pair<long, int>;

void solve() {

    
}

vector<pii> adjs[MAXN];
long dis[MAXN];
bool vis[MAXN];

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    int n, m, h, s1, s2, c, u, v, w;

    cin >> n >> m >> h >> s1 >> s2 >> c;

    for(int i = 0; i < m; ++i)
      cin >> u >> v >> w, adjs[u].push_back({v, w}), adjs[v].push_back({u, w});
    
    auto dij = [&](int f){
      memset(dis, 0x3f, sizeof(dis));
      memset(vis, 0, sizeof(vis));

      dis[f] = 0;

      priority_queue<pii, vector<pii>, greater<pii>> q;

      q.push({0, f});

      while(!q.empty()){
        auto [cost, u] = q.top(); q.pop();
        if(vis[u])
          continue;
        vis[u] = true;

        for(const auto [v, d] : adjs[u]){
          if(d + dis[u] < dis[v]){
            dis[v] = d + dis[u];
            if(vis[v])
              continue;
            q.push({dis[v], v});
          }
        }
      }
    };

    long res = 0;
    
    dij(s1);
    res += dis[h] + dis[s2];
    dij(c);
    res += dis[s2] + dis[h];

    cout << res << '\n';
    

    
}

