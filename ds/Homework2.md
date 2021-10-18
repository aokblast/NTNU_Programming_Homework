## DS Homework 2

Student Number: 40947047s

Name: 洪盛益

### Problem 1

a. 

```pseudocode
procedure coeff(a, p):
	set i to 1
	
	while i <= avail:
		if a.expon[i] == p:
			return a.coef[i]
		end if
	
		set i to i + 1
		
	end while
	
	return -1
end procedure
```

b. 

​	i. $4$(One Integer) $* 100$(100 terms)  $* 2$(member in the struct(power and coef)) $ = 800$bytes

​    ii. $4$(One Integer) $* 3$(3 terms)  $* 2$(member in the struct(power and coef)) $ = 24$bytes

​	iii. $4$(One Integer) $* 2$(2 terms)  $* 2$(member in the struct(power and coef)) $ = 16$bytes

c.

​	Yes, it saves space when it is sparse. But if the polynomial is not sparse, it costs a lot of time because it runs $O(n)$ to get coefficent which $n$ is the non-zero terms in the polynomial.

## Problem 2

a.

| $B^T$  | row  | col  | value |
| ------ | ---- | ---- | ----- |
| $B[0]$ | 5    | 9    | 8     |
| $B[1]$ | 0    | 1    | 1     |
| $B[2]$ | 0    | 8    | 2     |
| $B[3]$ | 1    | 0    | 1     |
| $B[4]$ | 1    | 8    | -1    |
| $B[5]$ | 2    | 0    | 2     |
| $B[6]$ | 3    | 4    | -1    |
| $B[7]$ | 3    | 7    | 3     |
| $B[8]$ | 4    | 6    | 2     |

b.

| $AxB$ | row  | col  | val  |
| ----- | ---- | ---- | ---- |
| [0]   | 7    | 5    | 17   |
| [1]   | 0    | 0    | -2   |
| [2]   | 0    | 1    | 2    |
| [3]   | 0    | 2    | 2    |
| [4]   | 0    | 3    | -2   |
| [5]   | 1    | 0    | 4    |
| [6]   | 1    | 1    | -2   |
| [7]   | 2    | 0    | -1   |
| [8]   | 2    | 3    | -3   |
| [9]   | 3    | 0    | 4    |
| [10]  | 3    | 1    | -2   |
| [11]  | 4    | 1    | 1    |
| [12]  | 4    | 2    | 2    |
| [13]  | 5    | 0    | 2    |
| [14]  | 5    | 3    | -4   |
| [15]  | 6    | 1    | -1   |
| [16]  | 6    | 2    | -2   |
| [17]  | 6    | 3    | 6    |

c. 

| -2   | 2    | 2    | -2   | 0    |
| ---- | ---- | ---- | ---- | ---- |
| 4    | -2   | 0    | 0    | 0    |
| -1   | 0    | 0    | -3   | 0    |
| 4    | -2   | 0    | 0    | 0    |
| 0    | 1    | 2    | 0    | 0    |
| 2    | 0    | 0    | -4   | 0    |
| 0    | -1   | -2   | 6    | 0    |

