# Clock
## Synchronization
* External Synchronization $\forall i, |S(t)-C_i(t)| \lt D_e$
* Internal Synchronization $\forall i, |C_j(t)-C_i(t)| \lt D_i$
* External Synchronization leads to $\forall i, |C_j(t)-C_i(t)| \lt 2D_e$

## Faulty Clocks
* Monotonicity condition (clock never go back) : $t'>t \Rightarrow C(t') \gt C(t)$
* crash failed: not working
* arbitrary failure: any failure other than crash
* correct clock $\nrightarrow$ accurate clock

## Synchronization bounds
* $\forall step_i \in process, B_l \le T(step_i) \le B_u$
* $|T_{MsgTransmit} | \le B$
* External Synchronization

### Optimal clocks setting
n denotes #clocks 
$$clocks = t + (Trans_{max} + Trans_{min})(1-\frac{1}{n})$$

### Synchronized methods
#### Cristian's
range $[t+min, t+T_{round}-min]$

set to $t+T_{round}/2$

#### Berkeley algorithm
* Master collects clock value from all slvaes
* use round trip time to estimate slaves' clock's values
* take average
* send back drift to slaves for adjustment

### Networkd Time Protocol(NTP)
* hierarchical shape : Primary Server (connected to UTC) => Secondary servers => Synchronization subnet => users' conputer
* hierarchy can be reconfigure when failres occur

#### Synchronization Modes
* Multicast
* procedure call
* symmetric
    * $d_i = T_{i-2} - T_{i-3} + T_{i} - T_{i-1}$
    * $o_i = T_{i-2} - T_{i-3} + T_{i-1} - T{i}$
    * $o_i-d_i/2\leq o \leq o_i+d_i/2$ 

## Logical Clock
notation
* a happen-before b: $a \rightarrow b$
* a happen-before b on process p: $a \rightarrow_p b$

definition of $a\rightarrow b$
* if $a,b$ in same process and a happen first
* if $b$ is a receive event and $a$ is corresponding sending event
* if $a \rightarrow c$ and $ c \rightarrow b$

### Lamport's Logical Clocks
every process has its timestamps $L_i$, and change by following rules:
* $L_i = 0$
* $L_i$++, if events happen in process i
* $L_i = max(L_i, L_j) + 1$, if process i receive message from process j with its timestemp $L_j$

$L(e) \lt L(e')$ if (not only if) $e\rightarrow e'$ 

### Vector Clocks
every process maintain a vector of all process, and change vector elements by following rules:
* $V_i[j] = 0$
* $V_i[i]$++, if events happen in process i
* $V_i[j] = max(V_i[j], t_j) + 1$, if process i receive message from process j with its timestemp $t_j$

# Global State