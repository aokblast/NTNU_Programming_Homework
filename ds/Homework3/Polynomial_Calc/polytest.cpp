#include "poly.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
    Poly p;
    cout << p << endl;
    p[1] = 1;
    p[2] = 5;

    cout << p << endl;
    cout << p.size() << endl;

    p[0] = 1;



    cout << p << endl;
    cout << p.size() << endl;

    p.erase(1);
    p[100] = 5;


    cout << p << endl;
    cout << p.size() << endl;

    p[25] = 1;
    p.erase(20);



    cout << p << endl;
    cout << p.size() << endl;

    p.erase(0);

    cout << p << endl;
    cout << p.size() << endl;

    Poly p2;

    p2[1] = 20;
    p2[30] = 1;
    p2[25] = 50;
    p2[101] = 30;
    p2[2] = 5;
    
    cout << p << endl;
    cout << p2 << endl;

    Poly p3 = p - p2;

    cout << p3 << endl;

    p3.erase(101);
    p3.erase(100);
    p3.erase(25);

    cout << p3 << endl;

    p3.erase(30);
    p3.erase(2);
    p3[0] = 1;
    p3.erase(1);

    cout << p3 << endl;
}
