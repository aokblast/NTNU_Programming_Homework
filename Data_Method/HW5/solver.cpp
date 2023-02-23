#include <iostream>
#include <iomanip>

using namespace std;

const size_t MAXN = 1e2;
const int WIDTH = 4;

int n;
double arr[MAXN][MAXN];
double L[MAXN][MAXN];
double U[MAXN][MAXN];
double P[MAXN][MAXN];
double R[MAXN];


bool LU_Decompose() {

  memcpy(U, arr, sizeof(arr));
  memset(L, 0, sizeof(L));
  memset(P, 0, sizeof(P));


  for(int i = 0; i < n; ++i)
    L[i][i] = P[i][i] = 1;

  for(int j = 0; j < n; ++j) {
    int i = j;

    for(int r = j + 1; r < n; ++r)
      if(abs(U[r][j]) > abs(U[i][j]))
        i = r;

    if(i != j) {
      swap(P[j], P[i]);

      for(int k = j; k < n; ++k)
        swap(U[i][k], U[j][k]);

      for(int k = 0; k < j; ++k)
        swap(L[i][k], L[j][k]);
    }

    for(int i = j + 1; i < n; ++i) {
      L[i][j] = U[i][j] / U[j][j];
      for(int k = j; k < n; ++k) {
        U[i][k] = U[i][k] - L[i][j] * U[j][k];
      }
    }
  }

  return true;
}


void solve() {

}

int main() {
  cin >> n;

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j)
      cin >> arr[i][j];

    cin >> R[i];
  }



  LU_Decompose();

  cout << "P:\n";

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j)
      cout << setw(WIDTH) << P[i][j] << ' ';
    cout << '\n';
  }

  cout << "L:\n";

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j)
      cout << setw(WIDTH) << L[i][j] << ' ';
    cout << '\n';
  }

  cout << "U:\n";

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j)
      cout << setw(WIDTH) << U[i][j] << ' ';
    cout << '\n';
  }
}
