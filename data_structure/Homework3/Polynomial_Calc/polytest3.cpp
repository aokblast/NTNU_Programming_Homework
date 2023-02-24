#include <bits/stdc++.h>
#include "poly.h"

using namespace std;

int main(){
  Poly p("x^2+x+2");
  stringstream ss;
  ss << p;
  cout << ss.str() << endl;

}
