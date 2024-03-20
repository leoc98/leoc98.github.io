# Clock
## Synchronization
* External Synchronization $$\forall i, |S(t)-C_i(t)| \lt D_e$$
* Internal Synchronization $$\forall i, |C_j(t)-C_i(t)| \lt D_i$$
* External Synchronization leads to $$\forall i, |C_j(t)-C_i(t)| \lt 2D_e$$

## Faulty Clocks
* Monotonicity condition (clock never go back) : $$t'>t \Rightarrow C(t') \gt C(t)$$
* crash failed: not working
* arbitrary failure: any failure other than crash
* correct clock $$\nrightarrow$$ accurate clock

## Synchronization bounds
* $$\forall step_i \in process, B_l \le T(step_i) \le B_u$$
* $$|T_{MsgTransmit} | \le B$$
* External Synchronization

### Optimal clocks setting
n denotes #clocks 
$$
clocks = t + (Trans_{max} + Trans_{min})(1-\frac{1}{n})
$$

### Synchronized methods
#### Cristian's
range $$[t+min, t+T_{round}-min]$$

set to $$t+T_{round}/2$$

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
    * $$d_i = T_{i-2} - T_{i-3} + T_{i} - T_{i-1}$$
    * $$o_i = T_{i-2} - T_{i-3} + T_{i-1} - T{i}$$
    * $$o_i-d_i/2\leq o \leq o_i+d_i/2$$ 

## Logical Clock
notation
* a happen-before b: $$a \rightarrow b$$
* a happen-before b on process p: $$a \rightarrow_p b$$

definition of $$a\rightarrow b$$
* if $$a,b$$ in same process and a happen first
* if $$b$$ is a receive event and $$a$$ is corresponding sending event
* if $$a \rightarrow c$$ and $$ c \rightarrow b$$

### Lamport's Logical Clocks
every process has its timestamps $$L_i$$, and change by following rules:
* $$L_i = 0$$
* $$L_i$$++, if events happen in process i
* $$L_i = max(L_i, L_j) + 1$$, if process i receive message from process j with its timestemp $$L_j$$

$$L(e) \lt L(e')$$ if (not only if) $$e\rightarrow e'$$ 

### Vector Clocks
every process maintain a vector of all process, and change vector elements by following rules:
* $$V_i[j] = 0$$
* $$V_i[i]$$++, if events happen in process i
* $$V_i[j] = max(V_i[j], t_j) + 1$$, if process i receive message from process j with its timestemp $$t_j$$

# Global State
## Difficulties
Distributed 
* Garbage collection
* Deadlock detection
* Termination detection
* Debugging

## Cuts
assemble a meaningful global state from local stes recorded at different times

### consistnet cut
$$
\forall e, e',e\in C, e'\rightarrow e \Rightarrow e' \in C
$$

### run
total ordering of all events in a global history that is consistent with each process' local history ordering.
#### linearization (consistent run)
ordering of evnets in a global history that is consitent with the happened-before relation

### reachable state
$$S'$$ is reachable from $$S$$, $$l$$ is a linearization of states
$$
\exist l, S\overset{l}{\rightarrow}S'
$$

## Snapshot
to recover from termination
### consist
* local snapshot of basic state of each process
* channel state of basic messages of each channel

### Assumption
* communication not failed
* process use channel FIFO
* distributed network strongly connected (reachable for any pair of processes)
* unspecific (snapshot algorithm can start at any process)
* not freezing while snapshot running
// TODO: what if process can handle message concurrently

### Chandy and Lamport’s Snapshot Algorithm
#### control message (marker) handle 
* the inital process pretend received a marker
* when first time received, record states and receving channel (empty), send out a marker over all outgoing channel;
* when not first time received (from other channel), record all messages from start record (the first time received)

#### complexity
assume there are $$e$$ edges and the diameter of network is $$d$$
* #messages $$\in O(e)$$
* time consuming $$\in O(d)$$

#### consistent snapshots
* presnapshot event
* postsnapshot event
* reachability theorem: for a linerization series of event, exist a permutation of Sys which can be divided into presnapshot evnets and postsnapshot evnets from a point, that makes $$S_{init} \overset{pre}{\rightarrow} S_{snap} \overset{post}{\rightarrow} S_{final}$$

### Monitoring Alogrithm (Marzullo-Neiger)
* use a monitor process that connects to all other processes and doesn't interfere other processes

TODO: $$\phi$$ meaning and lattice and the procedure of Marzullo-Neiger

# Coordination
## Assumption
* reliable channels
* do not rely on others for communication 
* crash and abnormally action is possible

### Failure detector
suspected failed if not receive message for T+D (D = maximum transmission delay)
TODO: listen to lecture for his murmur

### Mutal Exclusion
* assumption
    * only one critical section
    * asynchronous system
    * communication relibale 
    * process will not failed
* Application level Protocol
    * enter (try to access, may block)
    * access (critical)
    * exit
* Expectation
    * safety, only one process access at a moment
    * liveness, all access will exit in the end, and all enter will not permenant block (dead lock or starvation)
    * ordering, first enter first access

### Evaluation 
* bandwidth consumption
* client delays
* throughput
    * synchronization delay


### Algorithm
* Central Server Algorithm
    * central server will queue the enter request 
    * safety and liveness is guaranteed (due to sumption never failed)
    * ordering is not guaranteed (due to the network delay)
    * cons: single server failed and bottlenecks

* Ring-based algorithm
    * pass access token through a logical ring
    * safety and liveness is guaranteed (due to sumption never failed)
    * ordering is not guaranteed 
    * evaluation
        * constant bandwidth consumption
        * entry delay 0 to N
        * synchronization delay 1 to N TODO:
        * waste bandwidth if no one enter

* Ricart and Agrawala's algorithm
    * use state to denotes current process
        * initialztion/exit: 
            * set state released
            * reply all request in queue
        * enter: 
            * set state to wanted
            * multicast request
            * wait until get all response
            * set state to held then access critical
        * when receive request:
            * if self access critical (state:held) queue the request
            * or if self enter and waiting for reply, compare lamport time stamp, if self early then queue
            * otherwise reply to this request
    * all expectations are guaranteed
    * evaluation
        * 2(n-1) message consumed in an enter request
        * synchronization delay: 1 message transmission time

* maekawa's Voting Algorithm
    * use a voting set model
        * $$\forall i,j, i\neq j, V_i \bigcap V_j \neq \phi$$ any two voting set overlap
        * $$\forall i, p_i \in V_i$$ processes are in their own set
        * $$\forall i,j, |V_i| = |V_j| =K $$ fairness
        * in optimal $$K \sim N^{\frac{1}{2}}$$
    * use vote to determine access
        * all process init to be state:released, noted:No
        * if one want to enter, multicast request to its voting set members, and wait until all response arrive to access (state: held, voted:no)
        * if one want to exit, multicast release to its voting set members
        * when one process get a request:
            * if it is accessing (state:held), queue request
            * if it has voted for some one to access (voted:yes), queue request
            * otherwise, response this request, and set voted:yes
        * when one process receive a release
            * if any request in queue, response to that request
            * otherwise set voted:no
    * safety is guaranteed
    * liveness is not guaranteed due to it might cause deadlocks