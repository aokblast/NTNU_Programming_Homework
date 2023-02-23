#include <iostream>
#include <cassert>

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


int main() {
  double A[MAXN][MAXN] = {{0}}, AT[MAXN][MAXN] = {{0}}, B[MAXN][MAXN]={{0}}, AT2[MAXN][MAXN]={{0}}, mat[MAXN][MAXN]={{0}};

  int n, m;

  cin >> n >> m;

  for(int i = 0; i < n; ++i)
    for(int j = 0; j <= m; ++j)
      if(j == m)
        cin >> B[i][0];
      else
        cin >> A[i][j];

  //do ATAX = ATB

  memcpy(AT, A, sizeof(A));
  transpose(AT, n, m);



  memcpy(AT2, AT, sizeof(AT));

  multiply(AT, A, m, n, m);
  multiply(AT2, B, m, n, 1);

  for(int i = 0; i < m; ++i)
    for(int j = 0; j <= m; ++j)
      if(j == m)
        mat[i][j] = AT2[i][0];
      else
        mat[i][j] = AT[i][j];

  assert(gauss(mat, m));

  for(int i = 0; i < m; ++i)
    cout<< mat[i][m] << '\n';


}
