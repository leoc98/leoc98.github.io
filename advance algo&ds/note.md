# Computational model
## RAM model
* Memory
    * infinite sequece of cells
    * each cells can store a word of $w$ bits
    * each cell is identified by an unique address
* CPU
    * fixed number of registers (with a word storage)
    * atomic OP
        * register (re-)initialization
        * arithmetric op
            * add
            * sub
            * mul
            * integer div
        * comparison & branch
        * memory access
# Complexity
the growth speed of operations (memory cells) compared to the growth speed of problem size

## asymptotic notation
* $Big-O$
$$f(n) \leq c_1 \times g (n)\ holds\ for\ all\ n \geq c_2 \Rightarrow f(n) \in O(g(n))$$
* $Big-\Omega$
$$f(n) \geq c_1 \times g (n)\ holds\ for\ all\ n \geq c_2 \Rightarrow f(n) \in \Omega(g(n))$$
* $Big-\Theta$
$$f(n) \in O(g(n))\ \&\&\ f(n) \in \Omega(g(n)) \Rightarrow f(n) \in \Theta(g(n))$$

# Treap
a data structure combined binary-search-Tree and min-Heap, its operation cost is **expected** $O(log(n))$
## Theorem
* sub-tree of treap is also a treap
* given search key and priority, the treap will be unique

## randomized treap
if the priority is generated randomly from a continuous ranges, the expected treap height will be bound by $O(log(n))$

### Depth analyze
denote $u_k$ as the priority of a node that kth smaller search key. We have 
$$u_1\lt u_2 \lt ... \lt u_k \lt ... \lt u_n$$

denote $[i\uparrow k]$ as if $u_i$ is a proper ancestor of $u_k$, (=1 if yes, =0 otherwise)
$$depth(u_k) = \sum^{n}_{i=1}[i\uparrow k]$$
because of $[i\uparrow k] = 0$, if $i$ is not proper ancestor of $k$
$$E[depth(u_k)]=\sum^{n}_{i=1}E[i\uparrow k] = \sum^{n}_{i=1}Pr[i\uparrow k]$$ 

denote $U(i,k)$ is set of node that $u_i,u_{i+1},...,u_k$ or $u_k,...,u_{i-1},u_i$ depend on the relationship of $i$ and $k$. we can get lemma 1
* lemma 1: For $i\neq k$, $[i\uparrow k] = 1$, if and only if $u_i$ has the smallest priority in $U(i,k)$
* obeservation 2: since every priority randomized drawn from a continuous range, node in $U(i,k)$ are equally likely to have the smallest priority

$$Pr[i\uparrow k] = \begin{cases}
\frac{1}{k-i+1} ,i < k \\
0 ,i = k \\
\frac{1}{i-k+1} ,i > k
\end{cases}
$$

using harmonic serious bound

$$ E[depth(u_k)]=\sum^{k-1}_{j=2} \frac{1}{k-i+1} + \sum^{n}_{j=2} \frac{1}{i-k+1} \leq ln(k) + ln(n-k+1) \leq 2ln2 $$

# Amortized Analysis
## definition
$$\sum^m_{i=1} cost(\sigma_i) \leq \sum^m_{i=1} a(\sigma_i)$$
but note that it is unnecessary to have, it is also the difference between worse bound and amorized bound
$$ \forall i \in [1,m] \ cost(\sigma_i) \leq a(\sigma_i)$$

## why amorized
when other people invoke some data structure's interface (many times), they will not care about the time consuming individual operation call, but the overall running time.

## methods
### prepaid argument
When do cheap operations, prepaid some extra time in account. When do expensive operations, withdraw from account to conpensate the high time consumption.

We need to show that the balance in account is positive (always?//todo) to support our prepaid correct.

### potential functions
Use $\Phi(S_i)$ to denote state of ith operation. ensure that
$$\Phi(S_i) \ge 0, \Phi(S_0) =0 $$
$$a(\sigma_i) = cost(\sigma_i)+\Phi(S_i)-\Phi(S_i-1)$$
$$\sum^n_{i=1} a(\sigma_i) = \sum^n_{i=1} cost(\sigma_i)+\Phi(S_i)-\Phi(S_0) \ge \sum^n_{i=1} cost(\sigma_i)$$

$\Delta \Phi(S)$ can be treated as prepaid (positive) and withdraw (negative) in prepaid argument.

## drawback
Not fit in some time critical process, since the worst case does exist.

## de-Amortized
Use physical inplementation inspired by prepaid argument to reduce expensive operation consuming by add some extra operation in cheap operation.
