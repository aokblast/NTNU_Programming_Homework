#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

using namespace std;

const int MAXN = 1e2 + 5;

bool gauss(double arr[MAXN][MAXN], int n) {
  for(int i = 0; i < n; ++i) {
    int r = i;
    for(int j = i + 1; j < n; ++j)
      if(arr[r][i] < arr[j][i])
        r = j;

    if(r != i)
      swap(arr[r], arr[i]);

    if(!arr[i][i])
      return false;

    double base = arr[i][i];

    for(int k = 0; k < n; ++k) {
      if(k == i) {
        continue;
      }

      double times = arr[k][i] / base;

      for(int j = i; j <= n; ++j)
        arr[k][j] -= times * arr[i][j];
    }

    for(int j = i; j <= n; ++j)
      arr[i][j] /= base;
  }

  return true;
}

void transpose(double arr[MAXN][MAXN], int n, int m) {
  double tmp[MAXN][MAXN] = {{0}};
  for(int i = 0; i < n; ++i)
    for(int j = 0; j < m; ++j)
      tmp[j][i] = arr[i][j];

  memcpy(arr, tmp, sizeof(tmp));
}

void multiply(double arr1[MAXN][MAXN], double arr2[MAXN][MAXN], int a, int b, int c) {
  double tmp[MAXN][MAXN] = {{0}};
  for(int i = 0; i < a; ++i)
    for(int j = 0; j < b; ++j)
      for(int k = 0; k < c; ++k)
        tmp[i][k] += arr1[i][j] * arr2[j][k];

  memcpy(arr1, tmp, sizeof(tmp));
}

void gms(double grid[MAXN][MAXN], double qRes[MAXN][MAXN], double rRes[MAXN][MAXN], int n, int m) {
  double q[MAXN][MAXN] = {{0}}, r[MAXN][MAXN] = {{0}};
  double cur[MAXN];
  double tr[MAXN][MAXN];
  double qt[MAXN][MAXN];
  double tmp[MAXN][MAXN];
  double a[MAXN][MAXN];

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < m; ++j)
      cur[j] = a[j][0] = grid[j][i];

    for(int j = 0; j < i; ++j) {
      memset(tmp, 0, sizeof(tmp));

      for(int k = 0; k < m; ++k)
        tmp[k][0] = q[k][j];


      memcpy(tr, tmp, sizeof(tmp));

      transpose(tr, m, 1);
      memcpy(qt, tr, sizeof(tr));
      multiply(tmp, tr, m, 1, m);
      multiply(tmp, a, m, m, 1);

      multiply(qt, a, 1, m, 1);

      r[j][i] = qt[0][0];

      for(int j = 0; j < m; ++j)
        cur[j] -= tmp[j][0];
    }

    double dis = 0;

    for(int j = 0; j < m; ++j)
      q[j][i] = cur[j], dis += (cur[j] * cur[j]);

    dis = sqrt(dis);
    r[i][i] = dis;

    for(int j = 0; j < m; ++j)
      q[j][i] = cur[j] / dis;

  }


  memcpy(qRes, q, sizeof(q));
  memcpy(rRes, r, sizeof(r));
}

int main() {
  double grid[MAXN][MAXN];

  double q[MAXN][MAXN], r[MAXN][MAXN];

  int n, m;

  cin >> n >> m;

  for(int i = 0; i < n; ++i)
    for(int j = 0; j < m; ++j)
      cin >> grid[i][j];

  gms(grid, q, r, m, n);

  for(int i = 0; i < n; ++i)
    for(int j = 0; j < m; ++j)
      cout << q[i][j] << " \n"[j == m - 1];

  for (int i = 0; i < m; ++i)
    for (int j = 0; j < m; ++j)
      cout << r[i][j] << " \n"[j == m - 1];
}
