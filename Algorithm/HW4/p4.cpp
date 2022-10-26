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

const int MAXN = 3e2 + 5;
const int MAXM = 1.2e5 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

struct E{
  int to, val;
};

int n, m, s, t;

bool vis[MAXN];
E edges[MAXM];
int head[MAXN];
int Next[MAXM];
int pre[MAXN];
int flow[MAXN];

int tot = 2, liu = 0, fee = 0;

bool spfa(){ 
	memset(vis, 0, sizeof(vis));
  vis[s] = 1;
  queue<int> q;
  q.push(s);
  flow[s] = 0x7f7f7f7f;
  while(!q.empty()){
    int u = q.front(); q.pop();
    for(int i = head[u]; i; i = Next[i]){
      if(edges[i].val){
        int v = edges[i].to;
        if(vis[v])
          continue;
        flow[v] = min(flow[u], edges[i].val);
        pre[v] = i;
        q.push(v);
        vis[v] = true;
        if(v == t)
          return true;
      }
    }
  }
  return false;


}

void inc(){
  int u = t;
  while(u != s){
    int p = pre[u];
    edges[p].val -= flow[t];
    edges[p ^ 1].val += flow[t];
    u = edges[p ^ 1].to;
  }
  liu += flow[t];
}


void EK(){
	while(spfa()){
    inc();
	}
}

void insert(int u, int v, int f){
  edges[tot].to = v, edges[tot].val = f, Next[tot] = head[u], head[u] = tot++;
  edges[tot].to = u, edges[tot].val = 0, Next[tot] = head[v], head[v] = tot++;
}


int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    int n, m, c;
    cin >> n >> m;

    s = 0, t = n + 1;

    for(int i = 1; i <= n; ++i)
      cin >> c, insert(s, i, c);

    for(int i = 1; i <= n; ++i)
      cin >> c, insert(i, t, c);

    int u, v;
    while(m--){
      cin >> u >> v >> c;
      insert(u, v, c);
      insert(v, u, c);
    }

    EK();

    cout << liu << '\n';
    
     


}

