# graph_val.h
A simple library to validate graphs.

## class Graph
Represents an undirected graph.
All states (connected, bipartite, ...) are evaluated on the fly.

#### Methods
**`Graph(Integer l, Integer r)`**  
**`Graph(Integer n)`**  
The constructor.
Creates a graph with vertex ids in [l, r) or [0, n) respectively.

**`std::set<Integer> operator[](Integer x) const`**  
Returns all neighbours of the vertex `x`.

**`Integer degree(Integer x) const`**  
Returns the number of neighbours of the vertex `x`.

**`std::size_t size() const`**  
Returns the number of vertices as `std::size_t` (unsigned).

**`Integer nodeCount() const`**  
Returns the number of vertices as `Integer` (signed).

**`Integer edgeCount() const`**  
Returns the number of vertices as `Integer` (signed).

**`void addEdge(Integer a, Integer b)`**  
Adds an edge between `a` and `b`.

**`bool hasEdge(Integer a, Integer b) const`**  
Returns `true` iff `a` and `b` are connected by an edge.

**`std::optional<Integer> hasSelfloop() const`**  
Returns an vertex id which has an selfloop, should a selfloop exist.

**`std::optional<std::pair<Integer, Integer>> hasMultiedge() const`**  
Returns an edge `{a, b}` which exists twice, should a multiedge exist.

**`bool isConnected() const`**  
Returns `true` iff the graph is connected.

**`bool isForest() const`**  
Returns `true` iff the graph has no cycles i.e. forms a forest.

**`bool isTree() const`**  
Returns `true` iff the graph has no cycles and is connected i.e. forms a tree.

**`bool isBipartite() const`**  
Returns `true` iff the graph is bipartite.


## class DiGraph
Represents a directed graph.

#### Methods
**`DiGraph(Integer l, Integer r)`**  
**`DiGraph(Integer n)`**  
The constructor.
Creates a graph with vertex ids in [l, r) or [0, n) respectively.

**`std::set<Integer> operator[](Integer x) const`**  
**`std::set<Integer> inEdges(Integer x) const`**  
Returns all vertices which have an edge starting/ending at the vertex `x`.

**`Integer outDegree(Integer x) const`**  
**`Integer inDegree(Integer x) const`**  
Returns the number of edges starting/ending at the vertex `x`.

**`std::size_t size() const`**  
Returns the number of vertices as `std::size_t` (unsigned).

**`Integer nodeCount() const`**  
Returns the number of vertices as `Integer` (signed).

**`Integer edgeCount() const`**  
Returns the number of vertices as `Integer` (signed).

**`void addEdge(Integer a, Integer b)`**  
Adds an edge from `a` to `b`.

**`bool hasEdge(Integer a, Integer b) const`**  
Returns `true` iff there is an edge from `a` to `b`.

**`bool hasReverse(Integer a, Integer b) const`**  
Returns `true` iff there is an edge from `b` to `a`.

**`std::optional<Integer> hasSelfloop() const`**  
Returns an vertex id which has an selfloop, should a selfloop exist.

**`std::optional<std::pair<Integer, Integer>> hasMultiedge() const`**  
Returns an edge `{a, b}` which exists twice, should a multiedge exist.

**`std::optional<std::vector<Integer>> isDAG() const`**  
Returns a topological order of the vertices iff the graph is a directed acyclic graph (DAG):


## class UnionFind
This is mainly used to perform all operations on a `Graph` efficiently on the fly.
#### Methods
**`UnionFind(Integer n)`**  
The constructor.
Takes a single integer, the number of vertices.

**`Integer findSet(Integer x) const`**  
Returns a representant in [0, n) for the component to which `x` belongs.

**`bool connected(Integer a, Integer b) const`**  
Returns `true` iff `a` and `b` belong to the same component.

**`bool unionSets(Integer a, Integer b)`**  
Connects the components to which `a` and `b` belong.
Does nothing if `a` and `b` are already connected.
