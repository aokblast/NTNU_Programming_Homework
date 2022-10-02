#include <iostream>

using namespace std;

const size_t MAXN = 1e2;

int n;
double arr[MAXN][MAXN + 1];
double L[MAXN][MAXN + 1];
double U[MAXN][MAXN + 1];

bool LU_Decompose() {
  memcpy(U, arr, sizeof(arr));
  memset(L, 0, sizeof(L));

  for(int i = 0; i < n; ++i)
    L[i][i] = 1;

  for(int j = 0; j < n; ++j) {
    for(int i = j + 1; i < n; ++i) {
      L[i][j] = U[i][j] / U[j][j];
      for(int k = j; k < n; ++k) {
        U[i][k] = U[i][k] - L[i][j] * U[j][k];
      }
    }
  }

  return true;
}


bool gauss() {
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
      if(k == i)
        continue;
      double times = arr[k][i] / base;
      for(int j = i; j < n + 1; ++j)
        arr[k][j] = arr[k][j] - times * arr[i][j];
    }
    for(int j = i; j < n + 1; ++j)
      arr[i][j] = arr[i][j] / base;
  }
  return true;
}

int main() {
  cin >> n;

  for(int i = 0; i < n; ++i)
    for(int j = 0; j <= n; ++j)
      cin >> arr[i][j];

  if(LU_Decompose()) {
    cout << "L:\n";
    for(int i = 0; i < n; ++i)
      for(int j = 0; j < n; ++j)
        cout << L[i][j] << " \n"[j == n - 1];

    cout << "U:\n";
    for(int i = 0; i < n; ++i)
      for(int j = 0; j < n; ++j)
        cout << U[i][j] << " \n"[j == n - 1];

  if(gauss()) {
    for(int i = 0; i < n; ++i)
      cout << arr[i][n] << '\n';
  }else{
    cout << "Answer not exist\n";
  }
}
