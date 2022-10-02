#include <iostream>
#include <cmath>
using namespace std;

const double EPS = 1e-8;

static int cmp(double d1, double d2) { 
  if(fabs(d1 - d2) <= EPS)
    return 0;
  return d1 - d2 > 0 ? -1 : 1;
}

int main() {
  double cur = 10, pre = 0;

  auto calc = [](double x) {
    return pow(2 * x + 2, 1.0 / 3);
  };

  while(cmp(cur, pre)) {
    pre = cur;
    cur = calc(cur);
  }

  cout << cur << '\n';
}
