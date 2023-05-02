//============================================================================//
// graph_gen.h                                                                //
//============================================================================//
// This header can be used to Generate graphs. We support:                    //
// - directed, undirected and weighted graphs                                 //
// - uniform tree sampling                                                    //
// - erdös renyi random graphs                                                //
// - random graphs with a given degree sequence                               //
// - random graphs with a power low distribution                              //
// - various operations to construct graphs                                   //
// This header requires validate.h zo to generate random graphs in a          //
// deterministic and reproducable fashion.                                    //
//============================================================================//
//version 1.0.3                                                               //
//https://github.com/mzuenni/icpc-header                                      //
//============================================================================//

#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include <cassert>
#include <cstddef>
#include <deque>
#include <iostream>
#include <ostream>
#include <optional>
#include <ostream>
#include <queue>
#include <random>
#include <set>
#include <utility>
#include <ranges>
#include <vector>
#include "validate.h"

namespace GraphDetail {
	struct NoData final {};

	// EdgeWrapper may be returned by some function
	template<typename E>
	struct EdgeWrapper final {
		const Integer from, to;
		E& data;
		E& operator*() {
			return data;
		}
		const E& operator*() const {
			return data;
		}

		friend std::ostream& operator<<(std::ostream& os, const EdgeWrapper<E>& e) {
			os << e.from << " " << e.to;
			if constexpr (!std::is_same_v<std::remove_cv_t<std::remove_reference_t<E>>, NoData>) os << " " << *e;
			return os;
		}
	};

	// EdgeType is the internel representation of an edge that should never get exposed!
	template<typename E>
	struct EdgeType final {
		// we use key(from, to) = from^to for efficiency
		// the key function must have the following properties:
		// key(a, b) == key(b, a)
		// to1 != to2 <=> key(from, to1) != key(from, to2)
		// from1 != from2 <=> key(from1, to) != key(from2, to)
		Integer fromXorTo, to;
		E data;
	};

	template<typename E>
	struct EdgePointerCompair final {
		using is_transparent = std::true_type;
		constexpr bool operator()(const EdgeType<E>* t, const EdgeType<E>* u) const {
			return t->fromXorTo < u->fromXorTo;
		}
		constexpr bool operator()(const EdgeType<E>* t, Integer u) const {
			return t->fromXorTo < u;
		}
		constexpr bool operator()(Integer t, const EdgeType<E>* u) const {
			return t < u->fromXorTo;
		}
	};

	template<typename E, bool DIR>
	class GraphType final {
		// do not change!
		static constexpr Integer DEFAULT_MIN_ID = 0;

		template<typename, bool> friend class GraphType;
		using EdgeTypeValue = E;
		using Edge = EdgeWrapper<E>;
		using ConstEdge = EdgeWrapper<const E>;
		using InternalEdge = EdgeType<E>;
		using Graph = GraphType<E, DIR>;

		std::deque<std::set<InternalEdge*, EdgePointerCompair<E>>> adj;
		std::deque<InternalEdge> edges;
		Integer minId;

		inline void assertNode(Integer id) const {
			if (id < minId or id >= minId + nodeCount()) {
				throw std::out_of_range("Index \"" + std::to_string(id) + "\" out of range [" + std::to_string(minId) + ", " + std::to_string(minId + nodeCount()) + ") !");
			}
		}

		auto addEdge(InternalEdge* e) {
			Integer from = e->fromXorTo ^ e->to;
			Integer to = e->to;
			if constexpr (!DIR) if (from != to) adj[to - minId].emplace(e);
			return adj[from - minId].emplace(e).first;
		}

		void buildAdj() {
			for (auto& e : edges) addEdge(&e);
		}
	public:
		// construct empty graph with vertices in [from, to)
		explicit GraphType(Integer from, Integer to) :
			adj(to - from),
			minId(from) {
			judgeAssert<std::invalid_argument>(from <= to, "Graph::from must not be greater than to!");
		}
		// construct empty graph with vertices in [0, n)
		explicit GraphType(Integer n = 0) :
			GraphType(DEFAULT_MIN_ID, n) {}

		// copy graphs without breaking pointers
		GraphType(const Graph& o) :
			adj(o.size()),
			edges(o.edges),
			minId(o.minId) {
			buildAdj();
		}
		Graph& operator=(const Graph& o) {
			if (&o != this) {
				adj.clear();
				adj.resize(o.size());
				edges = o.edges;
				minId = o.minId;
				buildAdj();
			}
			return *this;
		}
		Graph copy() const {
			return *this;
		}

		//basic graph parameters
		std::size_t size() const {
			return adj.size();
		}

		Integer nodeCount() const {
			return static_cast<Integer>(adj.size());
		}

		Integer edgeCount() const {
			return static_cast<Integer>(edges.size());
		}

		Integer degree(Integer id) const {
			return static_cast<Integer>(adj[id].size());
		}

		Integer getMinId() const {
			return minId;
		}

		Graph& setMinId(Integer newminId) {
			for (auto& n : adj) n.clear();
			for (auto& e : edges) {
				Integer from = e.fromXorTo ^ e.to;
				Integer to = e.to;
				from = from - minId + newminId;
				to = to - minId + newminId;
				e.fromXorTo = from ^ to;
				e.to = to;
			}
			minId = newminId;
			buildAdj();
			return *this;
		}

		Integer getNextId() const {
			return minId + nodeCount();
		}

		Graph& normalize() {
			return setMinId(DEFAULT_MIN_ID);
		}

		Graph range(Integer from, Integer to) const {
			judgeAssert<std::invalid_argument>(from <= to, "Graph: from must not be greater than to!");
			Graph res(from, to);
			from = std::max(static_cast<Integer>(0), from - minId);
			to = std::min(nodeCount(), to - minId);
			for (const auto& e : getEdges()) {
				if (e.from >= from and e.from < to and e.to >= from and e.to < to) {
					res.addEdge(e.from, e.to, e.data);
				}
			}
			return res;
		}

		// basic graph operations
		std::optional<Edge> findEdge(Integer from, Integer to) & {
			assertNode(from);
			assertNode(to);
			auto it = adj[from - minId].find(from ^ to);
			if (it != adj[from - minId].end()) {
				return Edge(from, to, (*it)->data);
			} else {
				return {};
			}
		}

		std::optional<ConstEdge> findEdge(Integer from, Integer to) const & {
			assertNode(from);
			assertNode(to);
			auto it = adj[from - minId].find(from ^ to);
			if (it != adj[from - minId].end()) {
				return ConstEdge(from, to, (*it)->data);
			} else {
				return {};
			}
		}

		// ids may change!
		Edge getEdgeById(std::size_t id) & {
			if (id >= edges.size()) {
				throw std::out_of_range("Index \"" + std::to_string(id) + "\" out of range [0, " + std::to_string(edges.size()) + ") !");
			}
			return Edge(edges[id].fromXorTo ^ edges[id].to, edges[id].to, edges[id].data);
		}

		ConstEdge getEdgeById(std::size_t id) const & {
			if (id >= edges.size()) {
				throw std::out_of_range("Index \"" + std::to_string(id) + "\" out of range [0, " + std::to_string(edges.size()) + ") !");
			}
			return ConstEdge(edges[id].fromXorTo ^ edges[id].to, edges[id].to, edges[id].data);
		}

		bool hasEdge(Integer from, Integer to) const {
			assertNode(from);
			assertNode(to);
			return adj[from - minId].find(from ^ to) != adj[from - minId].end();
		}

		Edge addEdge(Integer from, Integer to, const E& data = {}) & {
			assertNode(from);
			assertNode(to);
			auto it = adj[from - minId].find(from ^ to);
			if (it == adj[from - minId].end()) {
				edges.emplace_back(from ^ to, to, data);
				it = addEdge(&(edges.back()));
			} else {
				(*it)->data = data;
			}
			return Edge(from, to, (*it)->data);
		}

		bool eraseEdge(Integer from, Integer to) {
			assertNode(from);
			assertNode(to);
			auto it = adj[from - minId].find(from ^ to);
			if (it != adj[from - minId].end()) {
				InternalEdge* a = *it;
				InternalEdge* b = &(edges.back());
				adj[from - minId].erase(a);
				if constexpr (!DIR) adj[to - minId].erase(a);
				from = b->fromXorTo ^ b->to;
				to = b->to;
				adj[from - minId].erase(b);
				if constexpr (!DIR) adj[to - minId].erase(b);
				std::swap(*a, *b);
				edges.pop_back();
				if (a != b) addEdge(a);
				return true;
			} else {
				return false;
			}
		}

		bool eraseEdge(const Edge& edge) {
			return removeEdge(edge.from, edge.to);
		}

		std::ranges::view auto getEdges() const & {
			return std::views::all(edges)
				 | std::views::transform([](const InternalEdge& edge){
				return ConstEdge(edge.fromXorTo ^ edge.to, edge.to, edge.data);
			});
		}

		std::ranges::view auto getEdges() & {
			return std::views::all(edges)
				 | std::views::transform([](InternalEdge& edge){
				return Edge(edge.fromXorTo ^ edge.to, edge.to, edge.data);
			});
		}

		std::ranges::view auto operator[](Integer id) const & {
			assertNode(id);
			return std::views::all(adj[id-minId])
				 | std::views::transform([id](InternalEdge* edge){
				return ConstEdge(id, edge->fromXorTo ^ id, edge->data);
			});
		}

		std::ranges::view auto operator[](Integer id) & {
			assertNode(id);
			return std::views::all(adj[id-minId])
				 | std::views::transform([id](InternalEdge* edge){
				return Edge(id, edge->fromXorTo ^ id, edge->data);
			});
		}

		std::ranges::view auto getDegrees() const & {
			return std::views::all(adj)
				 | std::views::transform([](auto& n){
				return n.size();
			});
		}

		// more basic graph operations
		Graph& combine(const Graph& o) {
			if (&o != this) {
				while (minId > o.minId) {
					minId--;
					adj.emplace_front();
				}
				while (minId + nodeCount() < o.minId + o.nodeCount()) {
					adj.emplace_back();
				}
				for (const auto& e : o.edges) {
					addEdge(e.fromXorTo ^ e.to, e.to, e.data);
				}
			}
			return *this;
		}

		Graph& append(const Graph& o) {
			//assert(&o != this); // should work
			Integer diff = nodeCount() + minId - o.minId;
			adj.resize(nodeCount() + o.nodeCount());
			for (std::size_t i = 0, j = o.edges.size(); i < j; i++) {
				Integer to = o.edges[i].to;
				Integer from = o.edges[i].fromXorTo ^ to;
				addEdge(from + diff, to + diff, o.edges[i].data);
			}
			return *this;
		}

		Graph& permutateEdges() {
			for (auto& n : adj) n.clear();
			Random::shuffle(edges.begin(), edges.end());
			if constexpr(!DIR) {
				for (auto& e : edges) {
					if (Random::bit()) {
						e.to ^= e.fromXorTo;
					}
				}
			}
			buildAdj();
			return *this;
		}

		Graph& permutate(const std::vector<Integer>& perm) {
			judgeAssert<std::invalid_argument>(isPerm(perm, minId), "nGraph: not a permutation");
			for (auto& n : adj) n.clear();
			Random::shuffle(edges.begin(), edges.end());
			for (auto& e : edges) {
				if constexpr (!DIR) {
					if (Random::bit()) {
						e.to ^= e.fromXorTo;
					}
				}
				e.fromXorTo ^= e.to;
				e.fromXorTo = perm[e.fromXorTo - minId];
				e.to = perm[e.to - minId];
				e.fromXorTo ^= e.to;
			}
			buildAdj();
			return *this;
		}

		Graph& permutate() {
			std::vector<Integer> perm(size());
			std::iota(perm.begin(), perm.end(), minId);
			Random::shuffle(perm.begin(), perm.end());
			return permutate(perm);
		}

		GraphType<E, true>& reverse() {
			static_assert(DIR, "Graph: reverse() is only available on directed graphs!");
			for (auto& n : adj) n.clear();
			for (auto& e : edges) {
				e.to ^= e.fromXorTo;
			}
			buildAdj();
			return *this;
		}

		GraphType<E, false> undirected() const {
			static_assert(DIR, "Graph: undirected() is only available on directed graphs!");
			GraphType<E, false> res(minId, minId + nodeCount());
			res.edges = edges;
			res.buildAdj();
			return res;
		}

		template<typename PRED>
		GraphType<E, true> directed(const PRED&& p) const {
			static_assert(!DIR, "Graph: directed() is only available on undirected graphs!");
			GraphType<E, true> res(minId, minId + nodeCount());
			for (const auto& e : getEdges()) {
				if (p(e.from, e.to)) res.addEdge(e.from, e.to, *e);
				if (p(e.to, e.from)) res.addEdge(e.to, e.from, *e);
			}
			return res;
		}

		GraphType<E, true> directed() const {
			return directed([](Integer /**/, Integer /**/){
				return true;
			});
		}

		Graph complement() const {
			Graph res(minId, minId + nodeCount());
			for (Integer i = 0; i < nodeCount(); i++) {
				for (Integer j = 0; j < i; j++) {
					if (!hasEdge(i + minId, j + minId)) {
						res.addEdge(i + minId, j + minId);
					}
					if constexpr (DIR) {
						if (!hasEdge(j + minId, i + minId)) {
							res.addEdge(j + minId, i + minId);
						}
					}
				}
			}
			return res;
		}

		// advanced graph operations
		Graph power(Integer k) const {
			Graph res(minId, minId + nodeCount());
			std::vector<bool> seen(nodeCount());
			for (Integer i = 0; i < nodeCount(); i++) {
				std::vector<std::pair<Integer, Integer>> todo = {{i, 0}};
				for (std::size_t j = 0; j < todo.size(); j++) {
					auto [c, d] = todo[j];
					if (d < k) {
						for (const auto& e : (*this)[c + minId]) {
							if (!seen[e.to - minId]) {
								seen[e.to - minId] = true;
								todo.emplace_back(e.to - minId, d + 1);
							}
						}
					}
				}
				for (auto [c, d] : todo) {
					seen[c] = false;
					if (i != c) {
						res.addEdge(i + minId, c + minId);
					}
				}
			}
			return res;
		}

		Graph& transitiveClosure() const {
			return power(nodeCount());
		}

		Graph product(const Graph& o) const {
			Graph res(nodeCount() * o.nodeCount());
			auto newId = [&](Integer a, Integer b){
				return (b - o.minId) * nodeCount() + (a - minId);
			};
			for (const auto& e1 : getEdges()) {
				for (const auto& e2 : o.getEdges()) {
					Integer from = newId(e1.from, e2.from);
					Integer to = newId(e1.to, e2.to);
					res.addEdge(from, to);
				}
			}
			return res;
		}

		Graph cartesian(const Graph& o) const {
			Graph res(nodeCount() * o.nodeCount());
			auto newId = [&](Integer a, Integer b){
				return (b-o.minId) * nodeCount() + (a-minId);
			};
			for (const auto& e : o.getEdges()) {
				for (Integer i = 0; i < nodeCount(); i++) {
					Integer from = newId(i + minId, e.from);
					Integer to = newId(i + minId, e.to);
					res.addEdge(from, to);
				}
			}
			for (const auto& e : getEdges()) {
				for (Integer i = 0; i < o.nodeCount(); i++) {
					Integer from = newId(e.from, i + o.minId);
					Integer to = newId(e.to, i + o.minId);
					res.addEdge(from, to);
				}
			}
			return res;
		}

		Graph strongProduct(const Graph& o) const {
			return o.cartesian(*this).combine(o.product(*this));
		}

		Graph lineGraph() const {
			static_assert(!DIR, "lineGraph() is only available on undirected graphs!");
			GraphType<Integer, false> tmp(minId, minId + nodeCount());
			Integer nextEdge = 0;
			for (const auto& e : getEdges()) {
				tmp.addEdge(e.from, e.to, nextEdge);
				nextEdge++;
			}
			Graph res(nextEdge);
			for (Integer i = 0; i < tmp.nodeCount(); i++) {
				auto tmpEdges = tmp[i];
				for (const auto& e1 : tmpEdges) {
					for (const auto& e2 : tmpEdges) {
						if (*e1 < *e2) res.addEdge(*e1, *e2);
					}
				}
			}
			return res;
		}

		friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
			os << g.nodeCount() << " " << g.edgeCount();
			for (const auto& e : g.getEdges()) os << '\n' << e;
			return os;
		}

	};
} // GraphDetail

using NoData = GraphDetail::NoData;

template<typename E = GraphDetail::NoData>
using Edge = GraphDetail::EdgeWrapper<E>;

template<typename E = GraphDetail::NoData>
using Graph = GraphDetail::GraphType<E, false>;

template<typename E = GraphDetail::NoData>
using DiGraph = GraphDetail::GraphType<E, true>;

// n independed vertices in range [0, n)
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> independent(Integer n) {
	return GraphDetail::GraphType<E, DIR>(n);
}

// n connected vertices in range [0, n)
// if directed all edges go from i to j with i < j
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> clique(Integer n) {
	// could be implemented as linegraph of a star.
	GraphDetail::GraphType<E, DIR> res(n);
	for (Integer j = 0; j < n; j++) {
		for (Integer i = 0; i < j; i++) {
			res.addEdge(i, j);
		}
	}
	return res;
}

// complete bipartite graph with partition [0, n) and [n, n+m)
// if directed all edges go from [0, n) to [n, n+m)
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> bipartite(Integer n, Integer m) {
	GraphDetail::GraphType<E, DIR> res(n+m);
	for (Integer i = 0; i < n; i++) {
		for (Integer j = 0; j < m; j++) {
			res.addEdge(i, n + j);
		}
	}
	return res;
}

// star with root = 0 and chilrden [1, leaves]
// if directed all edges go the root to [1, leaves)
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> star(Integer leaves) {
	return bipartite<E, DIR>(1, leaves);
}

// path 0-1-2-...-edges with vertices [0, edges]
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> path(Integer edges) {
	GraphDetail::GraphType<E, DIR> res(edges+1);
	for (Integer i = 0; i < edges; i++) {
		res.addEdge(i, i + 1);
	}
	return res;
}

// path 0-1-2-...-n-1-0 with vertices [0, n)
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> cycle(Integer n) {
	auto p = path<E>(n-1);
	p.addEdge(n - 1, 0);
	return p;
}

// wheel with root = 1 and outer vertices 1-2-...-outer-1 with vertices [0, outer]
template<typename E = NoData>
Graph<E> wheel(Integer outer) {
	return star<E>(outer).combine(path<E>(outer).setMinId(1));
}

// lolly graph with a path with p edges [0, p] and a clique [p, p+c) and
// p is the vertex where clique and path are connected
template<typename E = NoData>
Graph<E> lolly(Integer p, Integer c) {
	return path<E>(p).combine(clique<E>(c).setMinId(p));
}

// broom graph with a path with p edges [0, p] and a star [p, p+l]
// p is the vertex where star and path are connected
template<typename E = NoData>
Graph<E> broom(Integer p, Integer l) {
	return path<E>(p).combine(star<E>(l).setMinId(p));
}

// n times m grid
// 0-1-...-n-1
// | |      |
// n- ... -2*n-1
// |        |
// .        .
//          |
//        -n*m-1
template<typename E = NoData>
Graph<E> grid(Integer n, Integer m) {
	return path<E>(n-1).cartesian(path<E>(m-1));
}
template<typename E = NoData>
Graph<E> grid(Integer n) {
	return grid<E>(n, n);
}

// n dimensional hyper cube with vertices [0, 2^dimension)
// a-b <=> |a^b|=1
template<typename E = NoData>
Graph<E> hypercube(Integer dimension) {
	if (dimension < 2) return clique<E>(dimension+1);
	return hypercube<E>(dimension-1).cartesian(clique<E>(2));
}

// complete tree with root at 0 and all subtrees are consecutive
// vertices in [0, (children^{level+1}-1)/(children-1))
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> completeTree(Integer level, Integer children = 2) {
	if (level <= 2) return star<E, DIR>(children);
	GraphDetail::GraphType<E, DIR> res(1);
	GraphDetail::GraphType<E, DIR> sub = completeTree<E, DIR>(level - 1, children);
	for (Integer i = 0; i < children; i++) {
		Integer next = res.nodeCount();
		res.append(sub);
		res.addEdge(0, next);
	}
	return res;
}

// complete boolean lattice i->j <=> i|j==j
// vertices in [0, 2^level)
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> booleanLattice(Integer level) {
	GraphDetail::GraphType<E, DIR> res(1 << level);
	for (Integer i = 0; i < res.nodeCount(); i++) {
		for (Integer j = 1; j < res.nodeCount(); j*=2) {
			if ((i | j) == i) {
				res.addEdge(i, i^j);
			}
		}
	}
	return res;
}

// prefix of the rado graph i->j <=> i<j and j[i] == true;
// vertices in [0, n)
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> rado(Integer n) {
	GraphDetail::GraphType<E, DIR> res(n);
	for (Integer i = 0; i < n; i++) {
		for (Integer j = 0; (1 << j) < i; j++) {
			if ((i | (1 << j)) == i) {
				res.addEdge(i, j);
			}
		}
	}
	return res;
}

// generate a tree from a pruefer code
// values should be in [0, sz(code)+2)
template<typename E = NoData>
Graph<E> pruefer(const std::vector<Integer>& code) {
	Graph<E> res(code.size() + 2);
	std::vector<Integer> count(res.nodeCount());
	for (Integer x : code) count[x]++;
	Integer current = -1;
	Integer skip = -1;
	auto getNext = [&](){
		Integer oldSkip = skip;
		skip = -1;
		if (0 <= oldSkip and oldSkip <= current) return oldSkip;
		for (current++; count[current] > 0; current++) {}
		return current;
	};
	for (Integer x : code) {
		Integer y = getNext();
		res.addEdge(x, y);
		count[x]--;
		if (count[x] == 0) skip = x;
	}
	Integer x = getNext();
	Integer y = getNext();
	res.addEdge(x, y);
	return res;
}

// generate a random tree with vertices [0, n)
// if directed 0 is the root
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> randomTree(Integer n) {
	if constexpr (!DIR) {
		if (n <= 2) return clique<E>(n);
		std::vector<Integer> code(n - 2);
		for (Integer& x : code) x = Random::integer(n);
		return pruefer<E>(code);
	} else {
		Graph<E> tmp = randomTree<NoData, false>(n);
		DiGraph<E> res(n);
		auto dfs = [&tmp, &res](auto&& self, Integer cur, Integer pref) -> void {
			for (auto edge : tmp[cur]) {
				if (edge.to == pref) continue;
				res.addEdge(cur, edge.to);
				self(self, edge.to, cur);
			}
		};
		dfs(dfs, tmp.getMinId(), tmp.getMinId() - 1);
		return res;
	}
}

// generate a random graph (Erdös-Renyi)
// each edge exists with probability p
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> randomGraph(Integer n, Real p) {
	judgeAssert<std::domain_error>(0.0_real <= p and p <= 1.0_real, "randomGraph(): p must be in [0,1]!");
	GraphDetail::GraphType<E, DIR> res(n);
	for (Integer i = 0; i < n; i++) {
		Integer deg = Random::binomial(i, p);
		auto neighbours = Random::distinct(deg, 0, i);
		for (Integer j : neighbours) res.addEdge(i, j);
		if constexpr(DIR) {
			deg = Random::binomial(n - i - 1, p);
			neighbours = Random::distinct(deg, i + 1, n);
			for (Integer j : neighbours) res.addEdge(i, j);
		}
	}
	return res;
}

// generate a random graph (Erdös-Renyi)
// each edge exists with a given number of edges
template<typename E = NoData, bool DIR = false>
GraphDetail::GraphType<E, DIR> randomGraph(Integer n, Integer m) {
	GraphDetail::GraphType<E, DIR> res(n);
	Integer lim = n * (n-1);
	if constexpr (!DIR) lim /= 2;
	auto edges = Random::distinct(m, 0, lim);
	for (Integer x : edges) {
		Integer i = x / n;
		Integer j = x % n;
		if constexpr(!DIR) {
			if (j <= i) {
				i = n - 2 - i;
				j = n - 1 - j;
			}
		}
		res.addEdge(i, j);
	}
	return res;
}

// generate a random graph with linear growing degrees
template<typename E = NoData>
Graph<E> randomGrowingGraph(Integer n, Real p) {
	judgeAssert<std::domain_error>(0.0_real <= p and p <= 1.0_real, "randomGrowingGraph(): p must be in [0,1]!");
	Graph<E> res(n);
	for (Integer i = 0; i < n; i++) {
		Integer deg = p * Random::real(0, i+1);
		auto neighbours = Random::distinct(deg, 0, i);
		for (Integer j : neighbours) res.addEdge(i, j);
	}
	return res;
}

// generate a random graph with a given degree sequence
// (not uniform...)
template<typename E = NoData>
Graph<E> randomGraph(const std::vector<Integer>& degree, Integer averageFlips = 10) {
	Graph<E> res(std::ssize(degree));
	// havel hakimi in O(m*log(n))
	std::priority_queue<std::pair<Integer, Integer>> pq;
	for (Integer i = 0; i < std::ssize(degree); i++) pq.emplace(degree[i], i);
	while (!pq.empty()) {
		auto [d, c] = pq.top();
		pq.pop();
		if (std::ssize(pq) < d or d < 0) {
			throw std::invalid_argument("randomGraph(): Invalid degree sequence!");
		}
		std::vector<std::pair<Integer, Integer>> todo;
		for (Integer i = 0; i < d; i++) {
			auto [dd, cc] = pq.top();
			pq.pop();
			res.addEdge(c, cc);
			dd--;
			if (dd > 0) todo.emplace_back(dd, cc);
		}
		for (const auto& e : todo) pq.push(e);
	}
	// randomly flip edges O(m*log(n))
	for (Integer i = 0; i < averageFlips*res.nodeCount(); i++) {
		auto a = res.getEdgeById(Random::integer(res.edgeCount()));
		auto b = res.getEdgeById(Random::integer(res.edgeCount()));
		if (a.from == b.to or res.hasEdge(a.from, b.to)) continue;
		if (b.from == a.to or res.hasEdge(b.from, a.to)) continue;
		res.eraseEdge(a.from, a.to);
		res.eraseEdge(b.from, b.to);
		res.addEdge(a.from, b.to);
		res.addEdge(b.from, a.to);
	}
	return res;
}

#endif
