#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>

using namespace std;
const double EPS = 1e-6;

int cmp_double(double a, double b) {
  if((b - a) <= EPS)
    return 0;
  else return a > b ? -1 : 1;
}


int main() { 

  auto f = [&](double x) {
    return x - pow(x, 1.0 / 3) - 2;
  };

  double a, b;

  cin >> a >> b;

  while(cmp_double(a, b)) {
    double mid = (a + b) / 2;
    double mid_val = f(mid);
    double l = f(a);
    if(mid_val * l <= 0)
      b = mid;
    else
      a = mid;
  }


  cout << setprecision(6) << fixed << a << '\n';
}
