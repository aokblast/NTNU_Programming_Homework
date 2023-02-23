#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include <functional>
#include <tuple>

using namespace std;

const int MAXN = 1e2 + 5;
const double EPS = 1e-8;

using functype = function<double(const vector<double> &x)>;
vector<vector<functype>> jacobian;
vector<functype> fs;
int n, m;

double arr[MAXN][MAXN + 1];
double d[MAXN][MAXN];
double dtl[MAXN][MAXN];
double dtr[MAXN][MAXN];
double r[MAXN][MAXN];

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

static int cmp(const vector<double> &v1, const vector<double> &v2) {
  assert(v1.size() == v2.size());

  for(int i = 0; i < v1.size(); ++i) {
    if(fabs(v1[i] - v2[i]) <= EPS)
        continue;

    return v1[i] > v2[i] ? -1 : 1;
  }
  return 0;
}

vector<double> add(const vector<double> &d1, const vector<double> &d2) {
  assert(d1.size() == d2.size());

  vector<double> res(d1.size());

  for(int i = 0; i < d1.size(); ++i)
    res[i] = d1[i] + d2[i];

  return res;
}

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


vector<double> calc(const vector<double> &cur) {
  vector<double> res(m);
  memset(d, 0, sizeof(d));

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < m; ++j) {
      d[i][j] = jacobian[i][j](cur);
    }
  }
  memcpy(dtl, d, sizeof(d));
  transpose(dtl, n, m);
  memcpy(dtr, dtl, sizeof(d));
  multiply(dtl, d, m, n, m);

  for(int i = 0; i < n; ++i)
    r[i][0] = -(fs[i](cur));

  multiply(dtr, r, m, n, 1);

  for(int i = 0; i < m; ++i)
    dtl[i][m] = dtr[i][0];

  assert(gauss(dtl, m));

  for(int i = 0; i < m; ++i)
    res[i] = dtl[i][m];

  return res;
}


// Initialize
void initialize(const vector<function<double(const vector<double> &cst, const vector<double> &arg)>> &func_arr, const vector<vector<double>> &csts) {

  for(int i = 0; i < n; ++i)
    fs[i] = bind(func_arr[0], csts[i], placeholders::_1);

  for(int i = 0; i < n; ++i)
    for(int j = 1; j <= m; ++j)
        jacobian[i][j - 1] = bind(func_arr[j], csts[i], placeholders::_1);
}


void set_fn(vector<function<double(const vector<double> &cst, const vector<double> &arg)>> &func_arr, vector<vector<double>> &csts) {
  jacobian = vector<vector<functype>>(n, vector<functype>(m));
  fs.resize(n);

  func_arr.push_back(
      [](const vector<double> &cst, const vector<double> &arg) {
        double x = (cst[0] - arg[0]), y = (cst[1] - arg[1]), r = (cst[2] + arg[2]);
        return x * x + y * y - r * r;
        }
        );
  func_arr.push_back(
      [](const vector<double> &cst, const vector<double> &arg) {
        double x = (cst[0] - arg[0]), y = (cst[1] - arg[1]), r = (cst[2] + arg[2]);
        return -2 * x;
      }
      );
  func_arr.push_back(
      [](const vector<double> &cst, const vector<double> &arg) {
        double x = (cst[0] - arg[0]), y = (cst[1] - arg[1]), r = (cst[2] + arg[2]);
        return -2 * y;
      }
      );
  func_arr.push_back(
      [](const vector<double> &cst, const vector<double> &arg) {
        double x = (cst[0] - arg[0]), y = (cst[1] - arg[1]), r = (cst[2] + arg[2]);
        return -2 * r;
      }
      );

  csts = {{-1, 0, 1}, {1, 0.5, 0.5}, {1, -0.5, 0.5}, {0, 1, 0.5}};
}


int main() {

  n = 4, m = 3;

  vector<function<double(const vector<double> &cst, const vector<double> &arg)>> func_arr;
  vector<vector<double>> csts;

  set_fn(func_arr, csts);

  initialize(func_arr, csts);
  vector<double> cur = {0, 0, 0}, pre;


  do {
    pre = cur;
    cur = add(pre, calc(cur));
  } while(cmp(cur, pre));

  for(int i = 0; i < m; ++i)
    cout << cur[i] << ' ';

  cout << '\n';
}






