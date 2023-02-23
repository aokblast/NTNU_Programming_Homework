#include <iostream>
#include <vector>

using namespace std;

int main() {

  int n;
  cin >> n;

  vector<double> xs(n);
  vector<double> ys(n);

  const auto lx = [&](int k, double x) {
    double res = 1;

    for(int i = 0; i < n; ++i) {
      if(k == i)
        continue;

      res = res * (x - xs[i]) / (xs[k] - xs[i]);
    }

    return res;
  };

  const auto lagrange = [&](double x) {
    double res = 0;

    for(int i = 0; i < n; ++i)
      res += lx(i, x)  * ys[i];

    return res;
  };

  for(int i = 0; i < n; ++i)
    cin >> xs[i] >> ys[i];

  int q;
  double x;
  cin >> q;

  while(q--) {
    cin >> x;
    cout << lagrange(x) << '\n';
  }

}
