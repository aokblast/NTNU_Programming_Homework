# DSA Non-Programming Problem Solving

## Problem 1 - Complexity

1. The loop will iterate $k$ times which $1+2+3...+k = \frac{(1 + k) * k}{2} \leq n$

   $k^2 + k \leq 2n \Rightarrow k^2 + k - 2n \leq 0 \Rightarrow k \leq \frac{-1 + \sqrt{1 + 8n}}{2} $(positive is not in consonance) $ = \Theta(\sqrt{n})$
   
2. The loop will iterate $k$ times which $m^{(2^k)} < n$

   $m^{(2^k)} < n \Rightarrow 2^k < \log_{m}{n} \Rightarrow k < log_{2}{(log_{m}{n})} \Rightarrow k<log_{2}{(\frac{log{n}}{log{m}})} \Rightarrow k < log_{2}{(\log{n})} - log_{2}{(log{m})} < \log_{2}{(\log{n})} = \Theta(\log{(\log{n})})$

3. The recursion will cause two condition:

   * n <= 87506055, the total step is $3^n$
   * n > 87506055, the total step is $4^{n - 87506055} 3^{87506055}$

   We only have to consider the condition that n is large enough because $3^n$ is true in a limited n value, so the total step $4^{n-87506055}3^{87506055} = \Theta(4^n)$

4. By the definition of $\Theta$ , $\Theta(k(n)) = q(n)$ exist $c_1\in\mathbb{N^+} \and c_2\in\mathbb{N^+}$ fulfill $c_1k(n)\leq q(n)\leq c_2k(n)$

   max$(f(n), g(n)) \leq f(n) + g(n) \leq$ $2$max($f(n),g(n)$)
   
   $c_1 = 1, c_2 = 2, k(n) = max(f(n), g(n)), q(n) = f(n) + g(n)$
   
   So, $f(n)+g(n) = \Theta(max(f(n),g(n)))$
   
   Q.E.D
   
5. By the definition of $\mathbb{O}$, $\mathbb{O}(k(n)) = q(n)$ exist $c \in \mathbb{N^+}$ fulfill $q(n)\leq ck(n)$

   $f(n) = \mathbb{O}(i(n))$ exist $c_1$ that $f(n)\leq c_1i(n)$

   $g(n) = \mathbb{O}(j(n))$ exist $c_2$ that $g(n) \leq c_2j(n)$

   $f(n)*g(n) \leq c_1i(n)c_2j(n) = c_1c_2i(n)j(n)$

   So,$f(n)*g(n) = \mathbb{O}(i(n)j(n))$

   $k(n) = i(n)*j(n), c = c_1c_2, q(n) = f(n)*g(n)$

   Q.E.D

6. By the definition of $\mathbb{O}$, $\mathbb{O}(k(n)) = q(n)$ exist $c \in \mathbb{N^+}$ fulfill $q(n)\leq ck(n)$

   $f(n) = \mathbb{O}(g(n))$ exist $c_1$ that $f(n) <= c_1g(n)$

   $2^{f(n)}\leq2^{c_1g(n)}$

   so, $2^{f(n)} = \mathbb{O}(2^{g(n)})$

   $c = c_1, q(n) = 2^{f(n)}, k(n) = 2^{g(n)}$

   Q.E.D

7. $\Theta(lg(n)) = ln(n) =\int_{x = 1}^{n}{\frac{1}{x}}{dx}$ By the definition of integral, $\int_{x = 1}^{n}{f(x)}{dx} <= f(1) + f(2) + f(3) ... + f(n - 1) = \Sigma_{k = 1}^{n - 1}f(k) \leq \Sigma_{k = 1}^{n}f(k)$

   $\Sigma_{k = 1}^{n}f(x) = 1 + \frac{1}{2} + \frac{1}{3}...+\frac{1}{n} \leq 2(f(2) + f(3) + f(4) + ... +f(n - 1) + f(n)) = 2(\frac{1}{2} + \frac{1}{3} +... + \frac{1}{n}) \leq 2\Sigma_{k = 2}^{n} \leq 2\int_{x = 1}^{n}f(x)dx$

   $\int_{x = 1}^{n}{\frac{1}{x}}dx \leq \Sigma_{k = 1}^{n}{\frac{1}{k}}$



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

