# KruskalMST

Kruskal's algorithm is a minimum-spanning-tree algorithm which finds an edge of the least possible weight that connects any two trees in the forest. It is a greedy algorithm in graph theory as it finds a minimum spanning tree for a connected weighted graph adding increasing cost arcs at each step. This means it finds a subset of the edges that forms a tree that includes every vertex, where the total weight of all the edges in the tree is minimized. If the graph is not connected, then it finds a minimum spanning forest (a minimum spanning tree for each connected component).

### Complexity
Kruskal's algorithm can be shown to run in O(E log E) time, or equivalently, O(E log V) time, where E is the number of edges in the graph and V is the number of vertices, all with simple data structures. 
* E is at most V^2 and log(V^2) = 2logV is OlogV
* Each isolated vertex is a separate component of the minimum spanning forest. If we ignore isolated vertices we obtain V ≤ 2E, so logV is O(logE).

We can achieve this bound as follows: first sort the edges by weight using a comparison sort in O(E log E) time; this allows the step "remove an edge with minimum weight from S" to operate in constant time. Next, we use a disjoint-set data structure to keep track of which vertices are in which components. We need to perform O(V) operations, as in each iteration we connect a vertex to the spanning tree, two 'find' operations and possibly one union for each edge. Even a simple disjoint-set data structure such as disjoint-set forests with union by rank can perform O(V) operations in O(V log V) time. Thus the total time is O(ElogE) = O(ElogV).

### Algorithm
```C#
A = ∅
foreach v ∈ G.V:
  MAKE-SET(v)
foreach (u, v) in G.E ordered by weight(u, v), increasing:
  if FIND-SET(u) ≠ FIND-SET(v):
    A = A ∪ {(u, v)}
    UNION(u, v)
return A
```
