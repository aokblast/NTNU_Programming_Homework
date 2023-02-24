#include "poly.h"
#include <bits/stdc++.h>

using namespace std;

int main(){
  Poly p1, p2;
  p1[1]  = p1[3] = p1[0] = 1;
  p1[2]  = 3;
  p2[2] = p2[1] = p2[0] = 1;
  Poly p3 = p1 / p2;
  cout << p3 << endl;
}
