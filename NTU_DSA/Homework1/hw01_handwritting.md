# DSA Non-Programming Problem Solving

## Problem 1 - Complexity

> Reference for drawing math graph:https://www.desmos.com/calculator

1. The loop will iterate $k$ times which $1+2+3...+k = \frac{(1 + k) * k}{2} \leq n$

   $k^2 + k \leq 2n \Rightarrow k^2 + k - 2n \leq 0 \Rightarrow k \leq \frac{-1 + \sqrt{1 + 8n}}{2} $(positive is not in consonance) $ = \Theta(\sqrt{n})$
   
2. The loop will iterate $k$ times which $m^{(2^k)} < n$

   $m^{(2^k)} < n \Rightarrow 2^k < \log_{m}{n} \Rightarrow k < log_{2}{(log_{m}{n})} \Rightarrow k<log_{2}{(\frac{log{n}}{log{m}})} \Rightarrow k < log_{2}{(\log{n})} - log_{2}{(log{m})} < \log_{2}{(\log{n})} = \Theta(\log{(\log{n})})$

3. The recursion will cause two condition:

   * n <= 87506055, the total step is $3^n$
   * n > 87506055, the total step is $4^{n - 87506055} 3^{87506055}$

   We only have to consider the condition that n is large enough because $3^n$ is true in a limited n value, so the total step $4^{n-87506055}3^{87506055} = \Theta(4^n)$

4. By the definition of $\Theta$ , $q(n) = \Theta(k(n))$ exist $c_1\in\mathbb{N^+} \and c_2\in\mathbb{N^+}$ fulfill $c_1k(n)\leq q(n)\leq c_2k(n)$

   max$(f(n), g(n)) \leq f(n) + g(n) \leq$ $2$max($f(n),g(n)$)
   
   So, $f(n)+g(n) = \Theta(max(f(n),g(n)))$
   
   $c_1 = 1, c_2 = 2, k(n) = max(f(n), g(n)), q(n) = f(n) + g(n)$
   
   Q.E.D
   
5. By the definition of $\mathbb{O}$, $q(n) = \mathbb{O}(k(n))$ exist $c \in \mathbb{N^+}$ fulfill $q(n)\leq ck(n)$

   $f(n) = \mathbb{O}(i(n))$ exist $c_1$ that $f(n)\leq c_1i(n)$

   $g(n) = \mathbb{O}(j(n))$ exist $c_2$ that $g(n) \leq c_2j(n)$

   $f(n)*g(n) \leq c_1i(n)c_2j(n) = c_1c_2i(n)j(n)$

   So,$f(n)*g(n) = \mathbb{O}(i(n)j(n))$

   $k(n) = i(n)*j(n), c = c_1c_2, q(n) = f(n)*g(n)$

   Q.E.D

6. By the definition of $\mathbb{O}$, $q(n) = \mathbb{O}(k(n))$ exist $c \in \mathbb{N^+}$ fulfill $q(n)\leq ck(n)$

   $f(n) = \mathbb{O}(g(n))$ exist $c_1$ that $f(n) <= c_1g(n)$

   $2^{f(n)}\leq2^{c_1g(n)}$

   so, $2^{f(n)} = \mathbb{O}(2^{g(n)})$

   $c = c_1, q(n) = 2^{f(n)}, k(n) = 2^{g(n)}$

   Q.E.D

7. By the definition of $\Theta$ , $q(n) = \Theta(k(n))$ exist $c_1\in\mathbb{N^+} \and c_2\in\mathbb{N^+}$ fulfill $c_1k(n)\leq q(n)\leq c_2k(n)$

   $ ln(n) = \Theta(lg(n)) =\int_{x = 1}^{n}{\frac{1}{x}}{dx}$ 

   Compare with sigma and integral in below figure
   
![image-20210410172638437](/home/blast/.config/Typora/typora-user-images/image-20210410172638437.png)
   
   The area below the red line is the area of $y = \frac{1}{x}$ which means $\int_{1}^{n+1}{\frac{1}{x}dx}$
   
   And the area of yellow line is $\frac{1}{1} + \frac{1}{2} + \frac{1}{3}...+\frac{1}{n}$ which means $\Sigma_{x = 1}^{n}{\frac{1}{x}}$ 
   
   The area of blue line is $\frac{1}{2} + \frac{1}{3} + \frac{1}{4}... + \frac{1}{n+1}$ which means $\Sigma_{x = 2}^{n + 1}{\frac{1}{x}dx}$
   
   Obviously, we can observe that $\frac{1}{2} + \frac{1}{3} + \frac{1}{4}... + \frac{1}{n+1}$ which means $\Sigma_{x = 2}^{n + 1}{\frac{1}{x}} \leq \int_{1}^{n+1}{\frac{1}{x}dx} \leq \Sigma_{x = 1}^{n}{\frac{1}{x}}$ 
   
   $\int_{1}^{n}{\frac{1}{x}dx} < \int_{1}^{n+1}{\frac{1}{x}dx}<\Sigma_{x = 1}^{n}{\frac{1}{x}} = 1 + \Sigma_{x = 2}^{n}\frac{1}{x} < 1 + \Sigma_{x = 2}^{n + 1}\frac{1}{x} \leq 1 + \int_{1}^{n+1}{\frac{1}{x}dx} \leq 2\int_{1}^{n}{\frac{1}{x}dx}$ when $n \geq \frac{e+\sqrt{e^2+4e}}{2}$
   
   So $\mathbb{ln}n = \int_{1}^{n}{\frac{1}{x}dx} \leq \Sigma_{k = 1}^{n}{\frac{1}{k}} \leq 2\int_{1}^{n}{\frac{1}{x}dx} = 2\mathbb{ln}n$
   
   So we proved that $\Sigma_{k = 1}^{n}{\frac{1}{k}} = \Theta({\mathbb{lg}n})$
   
   $c_1 = 1, c_2 = 2, k(n) = \mathbb{lg}n, q(n) = \Sigma_{k = 1}^{n}{\frac{1}{k}}$
   
   Q.E.D
   
8. By the definition of $\Theta$ , $q(n) = \Theta(k(n))$ exist $c_1\in\mathbb{N^+} \and c_2\in\mathbb{N^+}$ fulfill $c_1k(n)\leq q(n)\leq c_2k(n)$

   $\frac{n}{2}lg(\frac{n}{2}) = lg(\frac{n}{2}) + lg(\frac{n}{2}) + lg(\frac{n}{2}) ... $(total $\frac{n}{2}$ terms) $<= lg(1) + lg(2) + lg(3) ... + lg(n) = \mathbb{lg}(n!) \leq \mathbb{lg}(n^n) = n\mathbb{lg}(n)$

   So we proved that $\mathbb{lg}(n!) = \Theta(n\mathbb{lg}n)$

   $c_1 = \frac{1}{2}, c_2 = 1, q(n) = \mathbb{lg}(n!), k(n) = nln(n)$

   Q.E.D

9. By the definition of $\Theta$ , $q(n) = \Theta(k(n))$ exist $c_1\in\mathbb{N^+} \and c_2\in\mathbb{N^+}$ fulfill $c_1k(n)\leq q(n)\leq c_2k(n)$

   $f(n) = 2(f([\frac{n}{2}]) + n\mathbb{lg}n) = 2(2(f([\frac{n}{4}]+\frac{n}{2}\mathbb{lg}(\frac{n}{2}))) + n\mathbb{lg}n) = 2(n\mathbb{lg}n + n\mathbb{lg}(\frac{n}{2}) + n\mathbb{lg}(\frac{n}{4})... + 1)$, total $\log_2n$ terms.

   $\frac{\log_2{n}}{2}(n\lg{n}) =  2(\frac{\log_2n}{2})(n\lg(\sqrt{n})) = 2(n\lg{\sqrt{n}} + n\lg{\sqrt{n}} + n\lg{\sqrt{n}} ... + n\lg{\sqrt{n}})\leq 2(n\mathbb{lg}n + n\mathbb{lg}(\frac{n}{2}) + n\mathbb{lg}(\frac{n}{4})... + 1) = f(x)$

   $f(x)=2(n\mathbb{lg}n + n\mathbb{lg}(\frac{n}{2}) + n\mathbb{lg}(\frac{n}{4})... + 1) \leq 2(n\lg{n} + n\lg{n} ... + n\lg{n}) = 2log_2{n}(n\lg{n})$

   So, $\frac{nlog_2{(n)}\lg{n}}{2} \leq f(x) \leq 2nlog_2{(n)}\lg{n}$ 

   $f(n) = \Theta({\lg({n})n\mathbb{lg}(n)}) = \Theta(n(\mathbb{lg}n)^2)$

   $c_1 = \frac{1}{4}, c_2 = 1, q(n) = f(x), k(n) = n(\lg{n})^2$

   Q.E.D

10. By the definition of $\mathbb{O}$, $q(n) = \mathbb{O}(k(n))$ exist $c \in \mathbb{N^+}$ fulfill $q(n)\leq ck(n)$

    Because $f_k{(n)} = \mathbb{O}(n^2)$, so for each $f_k(n)$, there exist a $c_k$ that $f_k(n) \leq c_kn^2$

    So $\Sigma_{k = 1}^{n}f_k(n) = f_1(n) + f_2{(n)} ... + f_n{(n)} \leq c_1n^2 + c_2n^2 ... + c_kn^2 = (c_1 + c_2 ... + c_k)n^2$

    $\Sigma_{k = 1}^{n} f_k(n) = \mathbb{O}(n^2) = \mathbb{O}(n^3)$(Because $n^3 \geq n^2$ when $n>=1$)

    $c = (c_1 + c_2 + c_3... +c_k), q(n) = \Sigma_{k = 1}{n} f_k(n), k(n) = n^2$

    Q.E.D

11. $n = k_1m+r_1 \geq m+r1 \geq r_1+r_1 =2r_1$, gcd(m, n) = gcd($r_1$, m)   $\leq$ gcd($\frac{n}{2}$, m)

     $m = r_1k_2 + r_2 \geq r_1 + r_2 > 2r_2$, gcd($r_1$, m) = gcd($r_2$, $r_1$) $\leq$ gcd($\frac {m} {2}$, r1)

    Consider the worst case that we only decrease the value by divide two each times, and the recursion stop when m or n equals to 0, so we can get $log_2$(min($m$,$n$)) $= k$ while $k$ is the step we takes.

    So the time complexity is O(lg(m+n)) because  $log_2$(min($m$,$n$)) $ = O(\lg$(min($m$,$n$))) $= O(lg(m+n))$ 

    Q.E.D

## Problem 2 - Stack/Queue

> Reference for pseudo code: https://michaelchen.tech/blog/how-to-write-pseudocode/
>
> Reference for Amortized Time Complexity:https://medium.com/@satorusasozaki/amortized-time-in-the-time-complexity-of-an-algorithm-6dd9a5d38045
>
> Reference for how I draw picture:https://magiclen.org/kolourpaint/

1. ```pseudocode
   procedure reverse(source, help):
   	while source.size() > 2:
   		help.enqueue(source.front())
   		source.dequeue()
   	source.enqueue(source.front())
   	source.dequeue()
   	end while
   	totalEle <- help.size()
   	for i (0 to totalEle - 1) do
   		for j (0 to help.size() - 2) do
   			help.enqueue(help.front())
   			help.dequeue()
   		end for
   		source.enqueue(help.front())
   		help.dequeue()
   	end for
   ```
   
2. Prove:

   **while** in line 2 iterates $n - 2$ and times

   **for** in line 4-6 iterates $n - 2 + (n - 3) + (n - 4)...  + 1 = \frac{(n-2)(n - 1)}{2}$

   total iterates  $n-2 + \frac{(n - 2)(n-1)}{2} = \frac{n(n-2)}{2} = \mathbb{O}(n^2)$

3. A stack has a property that when we pop all the element from one stack and push to another stack, the order of the elements of the stack will be reversed. Below is an easy example. 
   

![image-20210410031219464](/home/blast/.config/Typora/typora-user-images/image-20210410031219464.png)

   With this property, we can use two stacks(assume being called with front and back) as two sides of a deqeu, and use a variable called dir(Here we use value 1 for front, -1 for back) to record the present direction. When we want to change the direction of operation, we only have to push all element from one to another, then we can let the bottom be the top of the stack, the top be the bottom of the stack. Thus we successfully operates on two sides of the deque.

   Here is the pseudo code

   ```pseudocode
front and back are two stack and dir is an int
deq is a deque
   struct deque:
   	front
   	back
   	dir
end struct
   
procedure push_front(x):
  	if dir == -1 then
   		while back.size() > 0:
   			front.push(back.front())
   			back.pop()
   		end while
   		dir = 1
   	end if
   	front.push(x)
   end procedure
   
procedure pop_front():
   	if dir == -1 then
   		while back.size() > 0:
   			front.push(back.front())
   			back.pop()
   		end while
   		dir = 1
   	end if
   	set num to front.front()
   	front.pop()
   	return num
end procedure
   
procedure push_back(x):
   	if dir == 1 then
   		while front.size() > 0:
   			back.push(front.front())
   			front.pop()
   		end while
   		dir = -1
   	end if
   	back.push(x)
end procedure
   
procedure pop_back(x):
   	if dir == 1 then
   		while front.size() > 0:
   			back.push(front.front())
   			front.pop()
   		end while
   		dir = -1
   	end if
   	set num to back.front()
   	back.pop()
   	return num
end procedure
   ```

4. Consider the worst time complexity, a deque has $n$ elements. And the present direction is back, so if we use push_front(), we have to reverse a stack which time complexity is $O(n)$. But after, all the push_front() operation is $O(1)$ because we don't have to reverse a stack. So in $n$ times consecutive push_front(), we use O(n) one time and $O(1)$ $n - 1$ times, the amortized time complexity for each push_front() is $O(1)$.

5. Same as the above, if the original direction is opposite, we have to use $O(n)$ one time and $O(1)$ $n - 1$ times in total $n$ times consecutive push_back(), the amortized time complexity for each push_back() is $O(1)$.

6. Same as the above, if the original direction is opposite, we have to use $O(n)$ one time and $O(1)$ $n - 1$ times in total $n$ times consecutive pop_front(), the amortized time complexity for each pop_front() is $O(1)$.

7. Same as the above, if the original direction is opposite, we have to use $O(n)$ one time and $O(1)$ $n - 1$ times in total $n$ times consecutive pop_back(), the amortized time complexity for each pop_back() is $O(1)$.

8. In consecutive push operations, when the stack isn't full, the time complexity of each push is $O(1)$, but when the stack is full, the time complexity is $O(n)$. So in $n$ times consecutive push, we use $O(1)$ n-1 times and $O(n)$ one times, so the amortized time complexity is $O(1)$.

## Problem 3 - Array / Linked Lists

