#include <iostream>
#include <cmath>
#include <cassert>
#include <functional>
#include <vector>
#include <tuple>

using namespace std;
const double EPS = 1e-8;
const int MAXN = 1e2 + 5;

using functype = function<double(const vector<double> &x)>;

int n;

vector<vector<functype>> jacobian;
vector<functype> fs;

double arr[MAXN][MAXN + 1];

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


static int cmp(const vector<double> &v1, const vector<double> &v2) {
  assert(v1.size() == v2.size());
  for(int i = 0; i < v1.size(); ++i) {
    if(fabs(v1[i] - v2[i]) <= EPS) {
        continue;
    }
    return v1[i] > v2[i] ? -1 : 1;
  }
  return 0;
}

vector<double> add(const vector<double> &d1, const vector<double> &d2) {
  assert(d1.size() == d2.size());

  vector<double> res(n);

  for(int i = 0; i < n; ++i)
    res[i] = d1[i] + d2[i];

  return res;
}

vector<double> calc(const vector<double> &cur) {
  vector<double> res(n);

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      arr[i][j] = jacobian[i][j](cur);
    }
  }

  for(int i = 0; i < n; ++i)
    arr[i][n] = -(fs[i](cur));

  assert(gauss());


  for(int i = 0; i < n; ++i)
    res[i] = arr[i][n];

  return res;
}




// Initialize
void initialize(const vector<function<double(const vector<double> &cst, const vector<double> &arg)>> &func_arr, const vector<vector<double>> &csts) {

  for(int i = 0; i < n; ++i)
    fs[i] = bind(func_arr[0], csts[i], placeholders::_1);

  for(int i = 0; i < n; ++i) {
    for(int j = 1; j <= n; ++j) {
        jacobian[i][j - 1] = bind(func_arr[j], csts[i], placeholders::_1);
    }
  }
}

void set_fn(vector<function<double(const vector<double> &cst, const vector<double> &arg)>> &func_arr, vector<vector<double>> &csts) {
    jacobian = vector<vector<functype>>(n, vector<functype>(n));
    fs.resize(n);

    func_arr.push_back(
    [](const vector<double> &cst, const vector<double> &arg) {
    double x = (cst[0] - arg[0]), y = (cst[1] - arg[1]);
    return x * x + y * y - arg[2] * arg[2];
    }
    );
    func_arr.push_back(
        [](const vector<double> &cst, const vector<double> &arg) {
          double x = (cst[0] - arg[0]), y = (cst[1] - arg[1]);
          return -2 * x;
        }
        );
    func_arr.push_back(
        [](const vector<double> &cst, const vector<double> &arg) {
          double x = (cst[0] - arg[0]), y = (cst[1] - arg[1]);
          return -2 * y;
        }
        );
    func_arr.push_back(
        [](const vector<double> &cst, const vector<double> &arg) {
          double x = (cst[0] - arg[0]), y = (cst[1] - arg[1]);
          return -2 * arg[2];
        }
        );

    csts = {{-1, 6}, {-2, -6}, {5, 0}};
}

int main() {
  n = 3;

  vector<function<double(const vector<double> &cst, const vector<double> &arg)>> func_arr;
  vector<vector<double>> csts;

  set_fn(func_arr, csts);

  initialize(func_arr, csts);
  vector<double> cur = {1, 1, 1}, pre;


  do {
    pre = cur;
    cur = add(pre, calc(cur));
  } while(cmp(cur, pre));


  for(int i = 0; i < n; ++i) {
    cout << cur[i] << '\n';
  }

}
