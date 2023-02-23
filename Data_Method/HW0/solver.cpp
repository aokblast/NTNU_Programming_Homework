#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;


int main() { 
  double x = 0, cur = 0;
  int n = 0;
  cin >> n;

  vector<double> terms(n + 1);

  for(int i = 0; i <= n; ++i)
    cin >> terms[i];
  
  cin >> x;

  for(int i = n; i >= 0; --i)
    cur = cur * x + terms[i];

  cout << setprecision(4) << fixed << cur << '\n';
}
