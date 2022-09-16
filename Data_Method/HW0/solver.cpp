#include <iostream>
#include <iomanip>

using namespace std;


int main() { 
  double x = 0, cur = 0;
  cin >> x;

  for(int i = 0; i <= 50; ++i)
    cur = cur * x + 1;

  cout << setprecision(4) << fixed() << cur  << '\n';
}
