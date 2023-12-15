# graph_gen.h
A simple library to generate graphs.
This library requires C++20


## Related Methods
**`Graph independent(Integer n)`**  
**`Graph<E> independent<E>(Integer n)`**  
**`DiGraph<E> independent<E, true>(Integer n)`**  
Generates a `Graph` with `n` independent vertices with ids in [0, n).

**`Graph clique(Integer n)`**  
**`Graph<E> clique<E>(Integer n)`**  
**`DiGraph<E> clique<E, true>(Integer n)`**  
Generates a `Graph` with `n` pairwise connected vertices with ids in [0, n).
If directed all edges go from i to j with i < j.

**`Graph bipartite(Integer n, Integer m)`**  
**`Graph<E> bipartite<E>(Integer n, Integer m)`**  
**`DiGraph<E> bipartite<E, true>(Integer n, Integer m)`**  
Generates a `Graph` with two sets [0, n) and [n, n+m).
Each of the first `n` vertices is connected to every of the last `m` vertices.
If directed all edges go from [0, n) to [n, n+m).

**`Graph randomBipartite(Integer n, Integer m, Real p)`**  
**`Graph<E> randomBipartite<E>(Integer n, Integer m, Real p)`**  
**`DiGraph<E> randomBipartite(Integer n, Integer m, Real p)`**  
**`DiGraph<E> randomBipartite<E, true>(Integer n, Integer m, Real p)`**  
Generates a `Graph` with two sets [0, n) and [n, n+m).
Two vertices `a` in [0, n) and `b` in [n, n+m) are connected with probability `p` i.e. this is a bipartite Erdös-Renyi random graph.

**`Graph randomBipartite(Integer n, Integer m, Integer k)`**  
**`Graph<E> randomBipartite<E>(Integer n, Integer m, Integer k)`**  
**`DiGraph<E> randomBipartite(Integer n, Integer m, Integer k)`**  
**`DiGraph<E> randomBipartite<E, true>(Integer n, Integer m, Integer k)`**  
Generates a `Graph` with two sets [0, n) and [n, n+m) and exactly k edges.
This is similiar to an Erdös-Renyi random bipartite graph with some specific `p`.

**`Graph path(Integer edges)`**  
**`Graph<E> path<E>(Integer edges)`**  
**`DiGraph<E> path<E, true>(Integer edges)`**  
Generates a `Graph` with edges+1 vertices in [0, edges].
The vertex is `i` is connected to vertex `i+1` [and for undirected also `i+1` to `i`].

**`Graph cycle(Integer n)`**  
**`Graph<E> cycle<E>(Integer n)`**  
**`DiGraph<E> cycle<E, true>(Integer n)`**  
Generates a `Graph` with n vertices in [0, n).
The vertex is `i` is connected to vertex `i+1`.
Additionally there is an edge between `n-1` and `0`.
In case of an undirected graph all edges exist in both directions

**`Graph star(Integer leaves)`**  
**`Graph<E> star<E>(Integer leaves)`**  
**`DiGraph<E> star<E, true>(Integer leaves)`**  
Generates a `Graph` with leaves+1 vertices in [0, leaves].
The vertex `0` is connected to every other vertex.
If directed all edges go the root to [1, leaves).

**`Graph wheel(Integer outer)`**  
**`Graph<E> wheel<E>(Integer outer)`**  
Generates a `Graph` with outer+1 vertices in [0, outer].
The vertex `0` is connected to every other vertex and the vertices in [1, outer] form a cycle.

**`Graph lolly(Integer p, Integer c)`**  
**`Graph<E> lolly<E>(Integer p, Integer c)`**  
Generates a `Graph` with p+c vertices in [0, p+c).
The first `p+1` vertices form a path and the last `c` vertices form a clique.

**`Graph<E> broom(Integer p, Integer l)`**  
Generates a `Graph` with p+l+1 vertices in [0, p+l].
The first `p+1` vertices form a path and the last `l+1` vertices form a star where vertex `p+1` is also the center of the star.
The last `l` vertices form the ends of the star.

**`Graph grid(Integer n)`**  
**`Graph<E> grid<E>(Integer n)`**  
**`Graph grid(Integer n, Integer m)`**  
**`Graph<E> grid<E>(Integer n, Integer m)`**  
Generates a `Graph` with n\*n respectively n\*m vertices in [0, n\*m).
```
0-1-...-n-1
| |      |
n- ... -2*n-1
|        |
.        .
         |
       -n*m-1
```

**`Graph hypercube(Integer dim)`**  
**`Graph<E> hypercube<E>(Integer dim)`**  
Generates a `Graph` with 2<sup>dim</sup> vertices in [0, 2<sup>dim</sup>).
Two vertices `a` and `b` are connected iff |a^b|=1.

**`Graph booleanLattice(Integer level)`**  
**`Graph<E> booleanLattice<E>(Integer level)`**  
**`DiGraph<E> booleanLattice<E, true>(Integer level)`**  
Generates a `Graph` with 2<sup>level</sup> vertices in [0, 2<sup>level</sup>).
Two vertices `a` and `b` are connected iff `b` contains exactly one bit less than `a`.
If the graph is directed only the edge from `a` to `b` exists.

**`Graph rado(Integer n)`**  
**`Graph<E> rado<E>(Integer n)`**  
**`DiGraph<E> rado<E, true>(Integer n)`**  
Generates a `Graph` with n vertices in [0, n).
Two vertices `a` and `b` are connected iff `a`th bit in `b` is set.
If the graph is directed only the edge from `a` to `b` exists.
The graph is a prefix of the infinite rado graph.

**`Graph completeTree(Integer level)`**  
**`Graph<E> completeTree<E>(Integer level)`**  
**`Graph completeTree(Integer level, Integer k)`**  
**`Graph<E> completeTree<E>(Integer level, Integer k)`**  
**`DiGraph<E> completeTree<E, true>(Integer level)`**  
**`DiGraph<E> completeTree<E, true>(Integer level, Integer children)`**  
Generates a `2` respectively `k` ary complete tree.
The root of the tree is vertex `0` and all subtrees have consecutive ids.
If the tree is directed only the edges from the parent to the children exists.

**`Graph randomTree(Integer n)`**  
**`Graph<E> randomTree<E>(Integer n)`**  
**`DiGraph<E> randomTree<E, true>(Integer n)`**  
Generates a uniformally random tree with n vertices in [0, n).
If directed vertex `0` is the root.

**`Graph randomGraph(Integer n, Real p)`**  
**`Graph<E> randomGraph<E>(Integer n, Real p)`**  
**`DiGraph<E> randomGraph(Integer n, Real p)`**  
**`DiGraph<E> randomGraph<E, true>(Integer n, Real p)`**  
Generates a `Graph` with n vertices in [0, n).
Two vertices `a` and `b` are connected with probability `p` i.e. this creates an Erdös-Renyi random graph.

**`Graph randomGraph(Integer n, Integer m)`**  
**`Graph<E> randomGraph<E>(Integer n, Integer m)`**  
**`DiGraph<E> randomGraph(Integer n, Integer m)`**  
**`DiGraph<E> randomGraph<E, true>(Integer n, Integer m)`**  
Generates a `Graph` with n vertices in [0, n) and exactly m edges.
This is similiar to an Erdös-Renyi random graph with some specific `p`.

**`Graph randomGrowingGraph(Integer n, Real p)`**  
**`Graph<E> randomGrowingGraph<E>(Integer n, Real p)`**  
Generates a `Graph` with n vertices in [0, n).
The degree distribution will be linear.

**`Graph<E> randomGraph(std::vector<Integer> degree)`**  
**`Graph<E> randomGraph(std::vector<Integer>& degree, Integer flips)`**  
Generates a `Graph` with a given degree sequence.
First a fixed graph with the given degree sequence gets generated and than n\*flips edges get randomly flipped.

**`Graph<Integer> ffWorstcase(Integer n)`**  
Generates a worstcase instance for Ford Fulkerson with dfs.
The graph contains 2\*n vertices and has 2^n flow from 0 to 2\*n-1.
See figure 2 in https://people.computing.clemson.edu/~bcdean/iterm.pdf.
Note that the specific order of edghes is required

**`Graph<std::pair<Integer, Integer>> mcmfWorstcase(Integer n)`**  
Generates a worstcase instance for various min cost max flow algorithms.
The graph contains 2\*n+2 vertices and has flow from 0 to 2\*n+1.
See Figure 4 in https://link.springer.com/article/10.1007/BF01580132.
Each edge has a capicity and cost assigned.


## class Edge&lt;E>
The template parameter `E` is optional.
If `E` is given the edge holds additional data of type `E`.
#### Member
**`Integer from`**  
**`Integer to`**  
**`E data`**  

#### Methods
**`E& operator*()`**  
**`E operator*() const`**  
Access `data`.


## class Graph&lt;E> | class DiGraph&lt;E>
The template parameter `E` is optional.
If `E` is given each edge holds additional data of type `E`.
> Almost all methods are available fo `Graph<E>` and `DiGraph<E>`.
> The methods will return a graph of the same type at the current graph.
#### Methods
**`Graph<E>()`**  
**`Graph<E>(Integer n)`**  
**`Graph<E>(Integer from, Integer to)`**  
The constructor.
Creates either an empty graph, or a graph with vertices in the range [0, n) respectively [from, to).

**`std::size_t size() const`**  
Returns the number of vertices as `std::size_t` (unsigned).

**`Integer nodeCount() const`**  
Returns the number of vertices as `Integer` (signed).

**`Integer edgeCount() const`**  
Returns the number of vertices as `Integer` (signed).

**`Integer degree(Integer x) const`**  
Returns the number of neighbours of the vertex `x`.

**`Integer getNextId() const`**  
The id the next vertex that gets appended will have.

**`Integer getMinId() const`**  
Returns the smallest id a vertex has.
If the graph is empty, the result is equal to `getNextId()`.

**`Integer setMinId(Integer x) const`**  
Change the ids of all vertices such that the smalles id will be `x`.

**`Graph<E>& normalize()`**  
Change the ids of all vertices such that the smalles id will be 0.

**`Graph<E> range(Integer from, Integer to) const`**  
Creates a new graph containing all vertices in the range [from, to) and copy all edges from the curent graph where both head and tail are in the range [from, to).

**`Edge<E> addEdge(Integer from, Integer to)`**  
**`Edge<E> addEdge(Integer from, Integer to, E data)`**  
Insert a new edge, or overwrite it if it already exists.
If the graph is undirected the reverse edge is also inserted and both edges hold the same data.

**`std::optional<Edge<E>> findEdge(Integer from, Integer to)`**  
**`std::optional<const Edge<E>> findEdge(Integer from, Integer to) const`**  
Find the edge between from and to, should it exists.

**`bool hasEdge(Integer from, Integer to) const`**  
Checks if a given edge already exists between from and to.

**`bool eraseEdge(Integer from, Integer to) const`**  
**`bool eraseEdge(Edge<E> e) const`**  
Erase an edge, should it exists.

**`Edge<E> getEdgeById(std::size_t id)`**  
**`const Edge<E> getEdgeById(std::size_t id) const`**  
Get an edge from an internal id. Internal ids are always in the range [0, edgeCount()).
> Note: Any opperation creating or deleting edges may change the id of any edge.

**`std::ranges::view<Edge<E>> getEdges()`**  
**`std::ranges::view<const Edge<E>> getEdges() const`**  
Get a view over all edges.
> Note: edges may appear in any order and the order may even change.

**`std::ranges::view<Edge<E>> operator[](Integer id)`**  
**`std::ranges::view<const Edge<E>> operator[](Integer id) const`**  
Get a view over all edges starting at id.
`edge.from` will always be id.
> Note: edges may appear in any order and the order may even change.

**`std::ranges::view<std::size_t> getDegrees() const`**  
Get the (unsorted) degree sequence of the graph.

**`Graph<E>& combine(Graph other)`**  
Insert all vertices and edges of `other` into this graph.
> Note: the ids of existing edges and vertices may change.

**`Graph<E>& append(Graph other)`**  
Append all vertices from `other` behind this graph.
The first vertex of other will get `getNextId()` as is.

**`Graph<E> product(Graph other) const`**  
Create the tensor prduct graph of this and other.

**`Graph<E> cartesian(Graph other) const`**  
Create the cartesian prduct graph of this and other.

**`Graph<E> strongProduct(Graph other) const`**  
Create the strong prduct graph of this and other i.e. the combination of tensor and cartesian product.

**`Graph lineGraph() const`**  
Creates a new graph where each edge of the current graph becomes a vertex.
Two vertices in the new graph are connected iff the original edges form a path of length two.
> The new graph will have no edge data.

**`Graph<E> power(Integer k) const`**  
Creates the power graph i.e. a graph that contains all paths of length at most `k` as edges.

**`Graph<E> transitiveClosure() const`**  
Connects any two vertices by an edge that are connected by a path of any length.

**`Graph<E>& permutateEdges()`**  
Change the internal ids of all edges.
If the graph is undirected the head and tail of some edges is also swapped.

**`Graph<E>& permutate()`**  
Calls `permutateEdges()` and additionally change all vertex ids.

**`Graph<E> complement() const`**  
Creates a new graph which contains all edges which do not belong to this graph.

#### Methods Graph&lt;E>
**`DiGraph<E> directed() const`**  
**`DiGraph<E> directed(predicate) const`**  
Creates a directed version which contains all edges in both directions of the current graph.
If a predicate is provided only edges for which `predicate(from, to)` or `predicate(from, to, edgedata)` is true will be added.
The predicate should have one of the two method signatures:  
  - `bool(Integer, Integer)`  
  - `bool(Integer, Integer, const E&)`  

#### Methods DiGraph&lt;E>
**`Graph<E> undirected() const`**  
Creates a new undirected graph which contains every edge that exists in at least one direction.

**`DiGraph<E>& reverse()`**  
Reverses the direction of every edge.
