#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

const int MAXN = 1e2;
const double EPS = 1e-4;
int n;
double mat[MAXN][MAXN];
double b[MAXN];
double LU[MAXN][MAXN];
double D_INV[MAXN][MAXN];


int comp(const vector<double> &v1, const vector<double> &v2) {
  assert(v1.size() == v2.size());

  for(int i = 0; i < v1.size(); ++i) {
    if(abs(v1[i] - v2[i]) < EPS)
      continue;
    return v1[i] > v2[i] ? -1 : 1;
  }

  return 0;
}

void calc(vector<double> &cur) {
  vector<double> tmp(cur.size(), 0.0);

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      tmp[i] += LU[i][j] * cur[j];
    }
  }

  for(int i = 0; i < n; ++i)
    tmp[i] = b[i] - tmp[i];

  fill(cur.begin(), cur.end(), 0.0);

  for(int i = 0; i < n; ++i)
    for(int j = 0; j < n; ++j)
      cur[i] += D_INV[i][j] * tmp[j];
}

vector<double> jacobi() {
  vector<double> res(n, 0), prev(n, 0);
  memset(LU, 0, sizeof(LU));
  memset(D_INV, 0, sizeof(D_INV));

  for(int i = 0; i < n; ++i)
    for(int j = 0; j < n; ++j)
      if(i == j) {
        D_INV[i][j] = 1.0 / mat[i][j];
      } else {
        LU[i][j] = mat[i][j];
      }

  do {
    prev = res;
    calc(res);
  }while(comp(prev, res));

  return res;
}


int main() {
  memset(mat, 0, sizeof(mat));

  cin >> n;

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j)
      cin >> mat[i][j];
    cin >> b[i];
  }

  auto res = jacobi();

  for(const double num : res)
    cout << num << '\n';
}
