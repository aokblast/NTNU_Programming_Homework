#include <iostream>

using namespace std;

const int MAXN = 1e2 + 5;

int main() {
  double cs[MAXN] = {0}, xs[MAXN] = {0}, ys[MAXN] = {0};

  double mat[MAXN][MAXN] = {{0}};

  int n;
  cin >> n;

  for(int i = 0; i < n; ++i)
    cin >> xs[i] >> ys[i], mat[i][i] = ys[i];

  for(int len = 2; len <= n; ++len) {
    for(int l = 0; l + len <= n; ++l) {
      int r = l + len - 1;
      mat[r][l] = (mat[r][l + 1] - mat[r - 1][l]) / (xs[r] - xs[l]);
    }
  }



  int q;
  cin >> q;
  while(q--) {
    double x = 0;
    cin >> x;
    double res = 0;

    for(int i = 0; i < n; ++i) {
      double tmp = mat[i][0];

      for(int j = 0; j < i; ++j)
        tmp *= x - xs[j];

      res += tmp;
    }

    cout << res << '\n';
  }

}
