# Computational model
## RAM model
* Memory
    * infinite sequence of cells
    * each cells can store a word of $$w$$ bits
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
* $$Big-O$$
$$
f(n) \leq c_1 \times g (n)\ holds\ for\ all\ n \geq c_2 \Rightarrow f(n) \in O(g(n))
$$
* $$Big-\Omega$$
$$
f(n) \geq c_1 \times g (n)\ holds\ for\ all\ n \geq c_2 \Rightarrow f(n) \in \Omega(g(n))
$$
* $$Big-\Theta$$
$$
f(n) \in O(g(n))\ \&\&\ f(n) \in \Omega(g(n)) \Rightarrow f(n) \in \Theta(g(n))
$$

# Treap
a data structure combined binary-search-Tree and min-Heap, its operation cost is **expected** $$O(log(n))$$
## Theorem
* sub-tree of treap is also a treap
* given search key and priority, the treap will be unique

## randomized treap
if the priority is generated randomly from a continuous ranges, the expected treap height will be bound by $$O(log(n))$$

### Depth analyze
denote $$u_k$$ as the priority of a node that kth smaller search key. We have 
$$
u_1\lt u_2 \lt ... \lt u_k \lt ... \lt u_n
$$

denote $$[i\uparrow k]$ as if $u_i$ is a proper ancestor of $u_k$$, (=1 if yes, =0 otherwise)
$$
depth(u_k) = \sum^{n}_{i=1}[i\uparrow k]
$$
because of $$[i\uparrow k] = 0$, if $i$ is not proper ancestor of $k$$
$$
E[depth(u_k)]=\sum^{n}_{i=1}E[i\uparrow k] = \sum^{n}_{i=1}Pr[i\uparrow k]
$$ 

denote $$U(i,k)$ is set of node that $u_i,u_{i+1},...,u_k$ or $u_k,...,u_{i-1},u_i$ depend on the relationship of $i$ and $k$$. we can get lemma 1
* lemma 1: For $$i\neq k$, $[i\uparrow k] = 1$, if and only if $u_i$ has the smallest priority in $U(i,k)$$
* obeservation 2: since every priority randomized drawn from a continuous range, node in $$U(i,k)$$ are equally likely to have the smallest priority

$$Pr[i\uparrow k] = \begin{cases}
\frac{1}{k-i+1} ,i < k \\
0 ,i = k \\
\frac{1}{i-k+1} ,i > k
\end{cases}
$$

using harmonic serious bound

$$
 E[depth(u_k)]=\sum^{k-1}_{j=2} \frac{1}{k-i+1} + \sum^{n}_{j=2} \frac{1}{i-k+1} \leq ln(k) + ln(n-k+1) \leq 2ln2 
$$

# Amortized Analysis
## definition
$$
\sum^m_{i=1} cost(\sigma_i) \leq \sum^m_{i=1} a(\sigma_i)
$$
but note that it is unnecessary to have, it is also the difference between worse bound and amortized bound
$$
 \forall i \in [1,m] \ cost(\sigma_i) \leq a(\sigma_i)
$$

## why amortized
when other people invoke some data structure's interface (many times), they will not care about the time consuming individual operation call, but the overall running time.

## methods
### prepaid argument
When do cheap operations, prepaid some extra time in account. When do expensive operations, withdraw from account to conpensate the high time consumption.

We need to show that the balance in account is positive (always?//todo) to support our prepaid correct.

### potential functions
Use $$\Phi(S_i)$$ to denote state of ith operation. ensure that
$$
\Phi(S_i) \ge 0, \Phi(S_0) =0 
$$
$$
a(\sigma_i) = cost(\sigma_i)+\Phi(S_i)-\Phi(S_i-1)
$$
$$
\sum^n_{i=1} a(\sigma_i) = \sum^n_{i=1} cost(\sigma_i)+\Phi(S_i)-\Phi(S_0) \ge \sum^n_{i=1} cost(\sigma_i)
$$

$$\Delta \Phi(S)$$ can be treated as prepaid (positive) and withdraw (negative) in prepaid argument.

## drawback
Not fit in some time critical process, since the worst case does exist.

## de-Amortized
Use physical inplementation inspired by prepaid argument to reduce expensive operation consuming by add some extra operation in cheap operation.

# Quake Heap
## tournament tree
* binary tree
* elements in leaves
* denoted all leaves in the same depth
* parent holds the smallest priority of its children

### operation
* link: $$O(1)$$ : add a new node with the lower priority of two **same height** tournament trees' root
* split: $$O(1)$$: cut the children with higher priority of root

## consistent
* a forest of tournament trees which partition the whole data set
* use pointer to denotes priority of nodes 
    * true data will has a pointer to the highest node of its priority 
    * all data with same priority has a pointer to true data
    * we can modify a set of nodes with same priority by changing true data ($$O(1)$$ operation)
* node of every height invariant
    * for $$\alpha \in [0.5,1]$, $n_{i-1} \leq \alpha n_i$$
    * when this property be violate, cut all nodes above (included) the lowest violated height
    * lemma 1: nodes bounded by $$O(n)$$
    * theorem 1: the space consumption is bounded by $$O(n)$$
    * lemma 2: height is bounded by $$O(log(n))$ <= $\alpha ^ {h_{max}}n\geq n_{height} \geq 1$$

## potential function
$$
\phi(S_i) = N + 3T+\frac{3}{2\alpha-1}B
$$
$$
\Delta \phi(S_i) = \Delta N + 3\Delta T+\frac{3}{2\alpha-1}\Delta B
$$
## operation
### Insertion
form a new tournamet tree (with one elements) and add it to the forest
* actual cost $$O(1)$$
* $$\Delta \phi$: $1+3*1+0$$
### Decrease key
cut the highest node that true data points to from its parent (if exist), then decrease key
* acutal cost $$O(1)$$
* $$\Delta \phi$: $0+3*1+\frac{3}{2\alpha-1}$$
### Delete min
denotes: $$\mathcal{L}$$ as 
1. find the smallest priority root in forest
    * actual cost: $$T^{(0)}$$, the number of trees in forest
2. delete all nodes with smallest priority
    * acutal cost: $$\mathcal{L}$$, the number of nodes to remove
3. while existing 2 trees with same height, link them
    * actual cost: $$\leq (T^{(0)} + \mathcal{L} - 1) - 1$, in the worst case, we have $(T^{(0)} + \mathcal{L} - 1)$$ trees before link, 1 tree after link
4. cut all heights that violate invariant
    * actual cost: $$\sum_{h'>h} n_{h'}$, where $h$$ is the highest height without violate invariant
5. return smallest priority

* total actual cost: $$2(T^{(0)} + \mathcal{L} - 1) + \sum_{h'>h} n_{h'}$$
* steps 1-3  
    * $$\Delta T = T^{(1)} - T^{(0)}$, where $T^{(1)}$$ is the # of trees in forest after link.
    * $$\Delta N \leq T^{(0)} - T^{(1)}$, since each cut of nodes will be conpensate if it cause a link, so the $-\Delta T$ bounds $\Delta N$$
    * $$\Delta B \leq 0$$, since bad nodes only being cut in this procedure

* step 4
    * $$\Delta T \leq n_h^{(0)}$, since the height $h$$ will be the root of trees after maintain invariant
    * $$\Delta N = -\sum_{h'>h} n_{h'}$$
    * $$\Delta B \leq -b^{(0)}_{h+1} \lt -(2\alpha-1)n_h^{(0)}$, since the $h+1$ nodes violate invariant, $b^{(0)}_{h+1} \geq 2n_{h+1} - n_{h} = (2\alpha -1) n_{h}$ (this derive from the relation of h-th height nodes and the root, bad/good nodes from h+1-th height nodes $n_h \geq \underbrace{0}_{root} + \underbrace{2(n_{h+1} - b_{h+1})}_{good\ nodes} + \underbrace{b_{h+1}}_{bad\ nodes}$$)

* amortized cost = $$2(T^{(0)} + \mathcal{L} - 1) + \sum_{h'>h} n_{h'} + 3(T^{(1)} - T^{(0)}) + T^{(0)} - T^{(1)} + 0 + 3n_h^{(0)} + -\sum_{h'>h} n_{h'} -\frac{3}{2\alpha-1}(2\alpha-1)n_h^{(0)} = 2(T^{(1)}+\mathcal{L} - 1) \leq 2(3h_{max})  \in O(log(n))$$ 

# Splay Tree
* simple
    * no auxiliary information to store
    * no balancing invariant (allow to be unbalanced)
    * binary tree
* powerful
    * each operation takes $$O(log(n))$$ amortized time
    * more efficient than $$O(log(n))$$ in reality

## Operation
### Splay(x)
* rotate the node x to be the root of the tree
    * use double roations
    * if zig-zig (or zag-zag)
        * rotate its parent
        * rotate itself
    * if zig-zag (or zag-zig)
        * rotate itself
        * rotate itself
    * one-step to root
        * rotate itself
#### amortized analyze
* form potential function
    * $$\phi(S_i)=\underset{x\in T}{\sum}r(x)$$
    * rank $$r(x) = log_2s(x)$$
    * sub-tree weight sum $$s(x) = \underset{u\in T(x)}{\sum}w(u)$$
    * $$w(x) = 1$$ in splay amortized analyze
* access lemma: the amortized cost of the splay(x) is bounded by $$
3(r^{(1)}(x)-r^{(0)}(x))+1
$$$ where $(0)$ denotes states before operation and $(1)$ denotes states after opration, (the +1 is for zig-zag or zag-zig case) and this lemma holds for all constant positive $w(x), x\in T$$

    * for one double rotation (i-th index ignored)
        * zig-zig (or zag-zag) case
            * $$\Delta(\phi) = r ^{(1)}(x) + r ^{(1)}(y) + r ^{(1)}(z) - r ^{(0)}(x) - r ^{(0)}(y)  - r ^{(0)}(z) $$
            * $$r ^{(1)}(x) = r ^{(0)}(z), r ^{(0)}(y) \ge r ^{(0)}(x), r ^{(1)}(y) \lt r ^{(1)}(x)$$ (by counting subtree number)
            * $$\Delta(\phi) \le r ^{(1)}(x) + r ^{(1)}(z) - 2r ^{(0)}(x)$$
            * $$\frac{r ^{(0)}(x) + r ^{(1)}(z)}{2} \leq log_2(\frac{s ^{(1)}(x)}{2}) = r ^{(1)}(x) - 1$ (using log function concave property $\frac{loga+logb}{2}\le log(\frac{a+b}{2})$$)
            * $$\Delta(\phi) \le 3(r^{(1)}(x)-r^{(0)}(x)) -2$$
            * actual cost = 2
        * zig-zag (or zag-zig) case

    * for overall rotation 
        * $$r_{i+1}^{(0)}(x) = r_{i}^{(1)}(x)$$
        * $$\Delta(\phi) = \sum_i 3(r_i^{(1)}(x)-r_i^{(0)}(x)) + 1= 3(r_n^{(1)}(x)-r_1^{(0)}(x)) + 1$$
        * $$r_i^{(1)}(x)=log_2s^{(0)}(x)\le log_2n$$
        * $$r_i^{(0)}(x)\geq log_2 w(x) = 0$$
        * thus, the access lemma is bound by $$O(log(n))$$

### Search(x)
* search x as BST
* splay(x) (splay(last_node_insearch) if x doesn't exist)

#### amortized
* the actual cost of one search(x) is not more than one splay(x), and the $$\Delta(\phi)$$ for a search operation is 0
* thus this operation is bounded by splay(x) which is $$O(log(n))$$

### Insert(x)
* insert x as BST
* splay(x)

#### amortized
* the acutal cost of one insert(x) is not more than one splay(x)
* the $$\Delta(\phi)$ is bounded by $r^{(1)}(root)-r^{(0)}(x) \in O(log(n))$ (by using telescoping sum, $r^{(0)}(y_{k+1})\geq r^{(1)}(y_{k})$$)
* thus this operation is bounded $$O(log(n))$$

### Delete(x)
* rotate x to the buttom
* remove x
* splay(x.parent) (if existed before)

## Optimalities
### Static
* tree once being constructed, can not be modified
* sufficiently long sequence
* only takes successful search
* => overall running time as good as any static tree
    * using shannon bound to support
    
#### amortized analyze
* let $$w(x) = p(x)$, where $p(x)$ is the relative frequentcy of $x$$ being search
    * $$p(x)*m \ge 1$$
    * $$\sum_{x\in T}p(x) = 1$$
* $$\Delta(\phi) = \phi(S_m) - \phi(S_0) \leq -\sum log \frac{1}{p(x)}$$
* using access lemma, amortized time for a long sequence search is bounded by $$\sum p(x)m(3log\frac{1}{p(x)}+1) = (3\sum p(x)m(log\frac{1}{p(x)}))+m$$
* so the $$acutal = amortized - \Delta \phi \in O(\sum (m\cdot p(x)log \frac{1}{p(x)}+m))$$
* using shannon theorem, this is optimal
### Dynamic
* better than static, conjectured (widely belive but not verified) dynamic optimal
* allowed updated tree