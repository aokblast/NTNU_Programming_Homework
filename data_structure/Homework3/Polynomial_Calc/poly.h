#ifndef POLY_H
#define POLY_H
#include <list>
#include <iostream>
#include <string>
#include <vector>

class Poly;

class term{
public:

    //constructor
    term(int expo, double val);
    term();

    //method
    bool operator<(const term &t) const;
    bool operator==(const term &t) const;
    term operator-()const;
    friend class Poly;
    friend std::ostream &operator<<(std::ostream &os, Poly &p);


private:

    //member
    int expo;
    double val;
};

class Poly
{

public:

    //alias
    using iterator = term *;
    using size_type = size_t;

    //constructor
    Poly();
    Poly(const std::string &str);

    //Destructor
    ~Poly();

    //method
    iterator begin();
    iterator end();

    iterator find(int index);
    iterator lower_bound(int index);
    size_type size();
    std::string str();

    void erase(int index);
    void sort();
    void clear();

    bool isZero();

    double& operator[](int val);
    Poly duplicate();

    Poly &operator+=(Poly &p);
    Poly operator+(Poly &p);
    Poly &operator-=(Poly &p);
    Poly operator-(Poly &p);
    Poly &operator*=(Poly &p);
    Poly operator*(Poly &p);
    Poly &operator/=(Poly &p);
    Poly operator/(Poly &p);
    Poly &operator%=(Poly &p);
    Poly operator%(Poly &p);



    //function
    friend std::ostream &operator<<(std::ostream &os, Poly &p);


private:
    term *elements;
    size_type sz = 0;

    void erase(iterator iter);
    void insert(iterator iter, term tmp);
};





#endif // POLY_H
