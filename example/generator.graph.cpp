//============================================================================//
// graph_gen.h example usage                                                  //
//============================================================================//

#include "../src/validate.h"
#include "../src/graph_gen.h"
using namespace Generator;

int main(int argc, char **argv) {
	Generator::init(argc, argv);			// initialize streams, rng and parse arguments

	//needs c++20
	// generate a hypercube on vertices [0, 8) append a second one with vertices [8, 16) and a cycle on vertices [16,19)
	Graph<NoData> res = hypercube(3).append(hypercube(3)).append(cycle(3));
	// connect the cycle to a face of the first hypercube
	res.addEdge(16, 0);
	res.addEdge(17, 1);
	res.addEdge(18, 2);
	// connect the cycle to a face of the second hypercube
	res.addEdge(16, 8);
	res.addEdge(17, 9);
	res.addEdge(18, 10);

	// randomly permutate edges and vertices
	res.permutate();
	// random DAG (since the vertex order is random)
	DiGraph<NoData> dag = res.directed([](Integer from, Integer to){
		return from < to;
	});

	// mage the graph use vertices [1, 20)
	dag.setMinId(1);
	// randomly permutate edges and vertices
	dag.permutate();
	// print:
	// nodeCount EdgeCount
	// edges...
	testOut << dag << std::endl;
}
