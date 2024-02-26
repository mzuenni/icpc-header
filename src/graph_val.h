//============================================================================//
// graph_val.h                                                                //
//============================================================================//
// This header can be used to validate graph problems.                        //
//============================================================================//
//version 1.1.1                                                               //
//https://github.com/mzuenni/icpc-header                                      //
//============================================================================//

#ifndef GRAPH_VALIDATE_H
#define GRAPH_VALIDATE_H

#include <vector>
#include <set>
#include "validate.h"

class UnionFind {
	mutable std::vector<Integer> unions;

	void linkSets(Integer a, Integer b) {
		if (unions[a] > unions[b]) unions[a] = b;
		else if (unions[b] > unions[a]) unions[b] = a;
		else {
			unions[a] = b;
			unions[b]--;
	}}

public:
	explicit UnionFind(Integer n) : unions(n, -1) {}

	Integer findSet(Integer n) const {
		if (unions[n] < 0) return n;
		return unions[n] = findSet(unions[n]);
	}

	bool connected(Integer a, Integer b) const {
		return findSet(a) == findSet(b);
	}

	bool unionSets(Integer a, Integer b) {
		bool res = !connected(a, b);
		if (res) linkSets(findSet(a), findSet(b));
		return res;
	}
};

class Graph;
class DiGraph;

namespace details {
	class BaseGraph {
		friend class ::Graph;
		friend class ::DiGraph;

		Integer offset, edges;
		std::vector<std::set<Integer>> adj;
		std::optional<Integer> selfloop;
		std::optional<std::pair<Integer, Integer>> multiedge;

		explicit BaseGraph(Integer l, Integer u) : offset(l), edges(0), adj(u-l) {}
		explicit BaseGraph(Integer n) : BaseGraph(0, n) {}

		void addEdge_(Integer a, Integer b) {
			if (a == b) selfloop = a;

			if (adj[a - offset].count(b) > 0) multiedge = {a, b};
			adj[a - offset].insert(b);

			edges++;
		}
	public:
		std::optional<Integer> hasSelfloop() const {return selfloop;}
		std::optional<std::pair<Integer, Integer>> hasMultiedge() const {return multiedge;}

		const std::set<Integer>& operator[](Integer id) const & {
			return adj[id - offset];
		}

		const std::set<Integer>&& operator[](Integer id) const && {
			return std::move(adj[id - offset]);
		}

		std::size_t size() const {
			return adj.size();
		}

		Integer nodeCount() const {
			return static_cast<Integer>(adj.size());
		}

		Integer edgeCount() const {
			return edges;
		}

		bool hasEdge(Integer a, Integer b) const {
			return adj[a - offset].count(b) > 0;
		}
	};
}

class Graph final : private details::BaseGraph {
	UnionFind uf, buf;
	Integer components;
	bool forest = true;
	bool bipartite = true;
public:
	explicit Graph(Integer l, Integer r) : details::BaseGraph(l, r), uf(r-l), buf(2*(r-l)), components(r-l) {}
	explicit Graph(Integer n) : Graph(0, n) {}

	using details::BaseGraph::hasSelfloop;
	using details::BaseGraph::hasMultiedge;
	using details::BaseGraph::operator[];
	using details::BaseGraph::size;
	using details::BaseGraph::nodeCount;
	using details::BaseGraph::edgeCount;
	using details::BaseGraph::hasEdge;

	void addEdge(Integer a, Integer b) {
		addEdge_(a, b);
		b -= offset;
		adj[b].insert(a);
		a -= offset;

		if (uf.unionSets(a, b)) components--;
		else forest = false;
		a *= 2;
		b *= 2;
		buf.unionSets(a, b ^ 1);
		buf.unionSets(a ^ 1, b);
		bipartite &= !buf.connected(a, a ^ 1);
	}

	Integer degree(Integer id) const {
		return static_cast<Integer>(adj[id - offset].size());
	}

	bool isConnected() const {return components <= 1;}
	bool isForest() const {return forest;}
	bool isTree() const {return components <= 1 and forest;}
	bool isBipartite() const {return bipartite;}
	bool areConnected(Integer a, Integer b) {return uf.connected(a - offset, b - offset);}
};

class DiGraph final : private details::BaseGraph  {
	std::vector<std::set<Integer>> rev;
	UnionFind uf;
	Integer root, rootCount;
public:
	explicit DiGraph(Integer l, Integer r) : details::BaseGraph(l, r), rev(r-l), uf(r-l), root(0), rootCount(r-l) {}
	explicit DiGraph(Integer n) : DiGraph(0, n) {}

	using details::BaseGraph::hasSelfloop;
	using details::BaseGraph::hasMultiedge;
	using details::BaseGraph::operator[];
	using details::BaseGraph::size;
	using details::BaseGraph::nodeCount;
	using details::BaseGraph::edgeCount;
	using details::BaseGraph::hasEdge;

	void addEdge(Integer a, Integer b) {
		addEdge_(a, b);
		b -= offset;
		if (rev[b].empty()) rootCount--;
		rev[b].insert(a);
		a -= offset;
		if (!uf.unionSets(a, b)) root = nodeCount();
		while (root < nodeCount() and !rev[root].empty()) root++;
	}

	const std::set<Integer>& inEdges(Integer id) const & {
		return rev[id - offset];
	}

	const std::set<Integer>&& inEdges(Integer id) const && {
		return std::move(rev[id - offset]);
	}

	Integer outDegree(Integer id) const {
		return static_cast<Integer>(adj[id - offset].size());
	}

	Integer inDegree(Integer id) const {
		return static_cast<Integer>(rev[id - offset].size());
	}

	bool hasReverse(Integer a, Integer b) const {
		return rev[a - offset].count(b) > 0;
	}

	std::optional<std::vector<Integer>> isDAG() const {
		std::vector<std::size_t> deg(adj.size());
		std::vector<Integer> order, todo;
		for (std::size_t i = 0; i < adj.size(); i++) {
			deg[i] = adj[i].size();
			if (deg[i] == 0) todo.push_back(i);
		}
		while (!todo.empty()) {
			order.push_back(todo.back());
			todo.pop_back();
			for (Integer y : adj[order.back()]) {
				y -= offset;
				deg[y]--;
				if (deg[y] == 0) todo.push_back(y);
			}
		}
		for (Integer& x : order) x += offset;
		if (order.size() == adj.size()) return order;
		return std::nullopt;
	}

	std::optional<Integer> isTree() const {
		if (rootCount == 1 and edgeCount() + 1 == nodeCount()) return root + offset;
		return std::nullopt;
	}

};

#endif
