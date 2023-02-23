#include <bits/stdc++.h>

using namespace std;

int main() {
  double x = 4;

  auto cnt = [](double x) {
    return 4 * x * x * x - 6 * x * x;
  };

  while(fabs(cnt(x)) > 1e-6) {
    x = x - 0.001 * cnt(x);
  }

  cout << x << '\n';
}
