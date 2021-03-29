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

   