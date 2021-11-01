# Polynomial Calculator

This is a simple Polynomial Calculator for arithmetic operation by Qt5.



## Polynomial class Design

This class implement a list to store the polynomial's non-zero terms and support five types of arithmetic operation

* Constructor

  ```cpp
  Poly::Poly() // default constructor initialize with 0
  Poly::Poly(const std::string &s) // initialize polynomial with string
  ```

* Supply method

  ```cpp
  iterator begin() // return an itertor from begin
  iterator end() // return an past-the-end iterator
  
  iterator find(int index) // return the iterator for the specific term whose exponent equals to index 
  iterator lower_bound(int index) // return the iterator for the specific term whose exponent equals to or large than index 
  size_type size() // return the total number of non-zero terms of the polynomial 
  std::string str() // return the string presentation of the polynomial
      
  void erase(int index) // erase the term with specific exponent equals to index
  void sort() // sort the polynomial
  void clear() // clear the polynomial
      
  bool isZero() //check if the polynomial is a zero polynomial
  ```
  
* Operator

  ```cpp
  double &operator[](int index) // return a reference of the value of a specific term whose exponent equals to index
      
  Poly &operator+=(Poly &p) 
  Poly &opeator+(Poly &p)
  Poly &operator-=(Poly &p) 
  Poly &opeator-(Poly &p)
  Poly &operator*=(Poly &p) 
  Poly &opeator*(Poly &p)
  Poly &operator/=(Poly &p) 
  Poly &opeator/(Poly &p)
  Poly &operator%=(Poly &p) 
  Poly &opeator%(Poly &p)
  ```

* friend function

  ```cpp
  friend std::ostream &opeator<<(std::ostream &os, Poly &p) // print the string presentation of the polynomial 
  ```



## Complexity analysis

Assume $A, B$ are two polynomial you want to perform the arithmetic operation, then $T_a, T_b$ is the total number of non-zero terms.

* Insert non-zero term(operator[]): 

  In each of the insertion, it cost $O(T_a+T_b)$ because we  call find() to iterate through the whole list to find the  correct index to insert so that it can  sorted by exponents .

* Add and minus:

  It just likes merge two sorted list, we  pass through the two polynomial and merge it together. So the time complexity  is $O((T_a + T_b) ^ 2)$. (Contain Insert)

* multiply:

  We used a nested loop to calculate. So the time complexity is $O(T_a*T_b)$. But when inserting(operator[]), it costs $O(T_a + T_b)$ to find the correct index to insert.

  So the total complexity is $O(T_a*T_b*(T_a+T_b)^2)$

* divide and remainder:

  We used a while loop to calculate until it's exponent is less then divisor's.

  By our analysis above, the most time-consuming operation is multiply(tmp2 *= p), so the time complexity of divide and remainder is $O((T_a - T_b) * T_a*T_b*(T_a+T_b) ^ 2)$

