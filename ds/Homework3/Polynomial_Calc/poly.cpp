#include "poly.h"
#include <algorithm>
#include <cmath>
#include <cstring>

/******** Poly Class ********/

// Constructor


Poly::Poly(){
    elements = new term[4000]();
    (this->operator[](0)) = 0;
}

Poly::Poly(const std::string &s){
    elements = new term[4000]();
    int expo = 0, ci = 0;
    double num = 0;
    bool inex = false, neg = false, hasx = false, isf = false;
    for(const char &c : s){
        if(c == ' ')continue;
        else if(c == '+' || c == '-'){
           if(expo == 0 && hasx)expo = 1;
           if(num == 0 && hasx)num = 1;
           if(num == 0)continue;
           this->operator[](expo) = neg ? (-1 * num) : num;
           neg = (c == '-');
           num = expo = inex = hasx = isf = ci = false;
        }else if(c == '^' && hasx){
            inex = true;
        }else if(isdigit(c)){
            switch(inex){
                case true:
                    expo = expo * 10 - '0' + c;
                    if(isf)throw "Floating point number cannot be exponent. Please retry\n";

                       break;
                case false:
                    if(hasx)throw "Wrong input. Please retry\n";
                    if(!isf)num = num * 10 - '0' + c;
                    else num = num + (c - '0') * pow(10, -(++ci));
                    break;
            }
        }else if(c == 'x' && !hasx){
            hasx = true;
            isf = false;
        }else if(c == '.' && !isf){
            isf = true;
        }else {
            throw "Wrong input. Please retry\n";
        }
    }
    (this->operator[](0)) = 0;
    if(expo == 0 && hasx)expo = 1;
    if(num == 0 && hasx)num = 1;
    (this->operator[](expo)) = neg ? (-1 * num) : num;

}

Poly::~Poly(){
}

// Iterator

Poly::iterator Poly::begin(){
    return elements;
}

Poly::iterator Poly::end(){
    return elements + sz;
}



// General operation

Poly::size_type Poly::size(){
    return sz + elements->val != 0;
}

Poly::iterator Poly::find(int index){
    for(iterator iter = begin(); iter < end(); ++iter){
        if(iter->expo == index){
            if(index == 0 && iter->val == 0)return end();
            return iter;
        }
    }
    return end();
}

Poly::iterator Poly::lower_bound(int index){
    for(iterator iter = begin(); iter < end(); ++iter){
        if(iter->expo >= index)return iter;
    }
    return end();
}

void Poly::erase(int index){
    iterator iter = find(index);
    if(index == 0){
        iter->val = 0;
        return;
    }
    if(iter != end()){
        erase(iter);
    }
}

void Poly::erase(iterator iter){
    iterator next = ++iter;
    for(; next != end(); ++iter){
        *iter = *next;
    }
    --sz;
}

void Poly::insert(iterator iter, term val){
    term tmp = *iter;
    *iter = val;

    if(end() == iter){
        ++sz;
        return;
    }
    ++iter;

    for(;iter <= end(); ++iter){
        std::swap(*iter, tmp);
    }
    *iter = tmp;
    ++sz;
}

void Poly::clear(){
    sz = 0;
    memset((void *)elements, 0, sizeof(elements));
    (this->operator[](0)) = 0;
}

void Poly::sort(){
    std::sort(elements, elements + sz);
}

bool Poly::isZero(){
    return sz == 1 && elements->val == 0;
}

std::string Poly::str(){
    std::string res;
    char buffer[100] = {0};
    for(iterator iter = end() - 1; iter >= elements + 1; --iter){
        sprintf(buffer, "%lg", iter->val);

        res += (iter->val != 1 ? buffer : "") + std::string("x");
        if(iter->expo != 1) res += '^' + std::to_string(iter->expo);
        res += " + ";
    }
    if(begin()->val || sz == 1) sprintf(buffer, "%lg", begin()->val), res += buffer;
    else for(int i = 0; i < 3; ++i) res.pop_back();

    return res;
}

// Operator

std::ostream &operator<<(std::ostream &os, Poly &p){
    os << p.str();
    return os;
}

double& Poly::operator[](int index){
    iterator iter = lower_bound(index);
    if(iter != end() && iter->expo == index)return iter->val;
    insert(iter, term(index, 0));

    return (iter)->val;
}


Poly &Poly::operator+=(Poly &p){
    iterator titer = this->begin();
    iterator piter = p.begin();
    while(titer != this->end() && piter != p.end()){
        if(titer->expo == piter->expo){
            titer->val += piter->val;
            if(!titer->val && titer->expo != 0){
                erase(titer);
                ++piter;
                continue;
            }
            ++titer;
            ++piter;

        }else if(titer->expo < piter->expo){
            ++titer;
        }else{
            insert(titer, *piter);
            ++titer;
            ++piter;
        }

    }
    while(piter != p.end()){
        elements[sz] = *piter;
        ++sz;
        ++piter;
    }
    return *this;
}

Poly Poly::operator+(Poly &p){
    Poly res = *this;
    res += p;
    return res;
}

Poly &Poly::operator-=(Poly &p){
    iterator titer = this->begin();
    iterator piter = p.begin();
    while(titer != this->end() && piter != p.end()){
        if(titer->expo == piter->expo){
            titer->val -= piter->val;
            if(!titer->val && titer->expo != 0){
                erase(titer);
                ++piter;
                continue;
            }
            ++titer;
            ++piter;
        }else if(titer->expo < piter->expo){
            ++titer;
        }else{
            insert(titer, -*piter);
            ++titer;
            ++piter;
        }

    }
    while(piter != p.end()){
        elements[sz] = -*piter;
        ++sz;
        ++piter;
    }
    return *this;
}

Poly Poly::operator-(Poly &p){
    Poly res = *this;
    res -= p;
    return res;
}

Poly &Poly::operator*=(Poly &p){
    Poly tmp = duplicate();
    this->clear();
    for(iterator titer = tmp.begin(); titer != tmp.end(); ++titer){
        for(iterator piter = p.begin(); piter != p.end(); ++piter){

            if(titer->val * piter->val)this->operator[](titer->expo + piter->expo) += titer->val * piter->val;
        }
    }
    return *this;
}

Poly Poly::operator*(Poly &p){
    Poly res = duplicate();
    res *= p;
    return res;
}

Poly &Poly::operator/=(Poly &p){
    if(p.isZero()){
        throw "Divide Zero is not permittable";
    }
    Poly tmp = duplicate();
    this->clear();
    if(--tmp.end()->expo < --p.end()->expo){
        (this->operator[](0)) = 0;
        return *this;
    }
    iterator titer = tmp.end() - 1, piter = p.end() - 1;
    while(titer->expo >= piter->expo){
        Poly tmp2;
        tmp2[titer->expo - piter->expo] = titer->val / piter->val;
        *this += tmp2;
        tmp2 *= p;
        tmp -= tmp2;
        titer = tmp.end() - 1;
        if(titer->expo == 0 && titer->val == 0)break;
    }
    return *this;
}

Poly Poly::operator/(Poly &p){
    Poly res=  duplicate();
    try{
        res /= p;
    }catch(const char *msg){
        throw msg;
    }
    return res;
}

Poly &Poly::operator%=(Poly &p){
    if(p.isZero()){
        throw "Divide Zero is not permittable";
    }
    if(--end()->expo < --p.end()->expo){
        return *this;
    }
    iterator titer = end() - 1, piter = p.end() - 1;
    while(titer->expo >= piter->expo){
        Poly tmp2;
        tmp2[titer->expo - piter->expo] = titer->val / piter->val;
        tmp2 *= p;
        *this -= tmp2;
        titer = end() - 1;
        if(titer->expo == 0 && titer->val == 0)break;
    }
    return *this;
}

Poly Poly::operator%(Poly &p){
    Poly res = duplicate();
    try{
        res %= p;
    }catch(const char *msg){
        throw msg;
    }
    return res;
}

Poly Poly::duplicate(){
   Poly res;
   res = *this;
   term *p = new term[4000]();
   memcpy((void *)p, (void *)this->elements, sizeof(*(this->elements)) * 4000);
   res.elements = p;
   return res;
}

/******** Term Class ********/



// Constructor

term::term(int expo, double val) : expo(expo), val(val){

}

term::term() : expo(0), val(0){

}

// Operator

bool term::operator<(const term &t) const{
    return expo < t.expo;
}

bool term::operator==(const term &t) const{
    return expo == t.expo;
}

term term::operator-()const{
    term result = *this;
    result.val = -result.val;
    return result;
}
