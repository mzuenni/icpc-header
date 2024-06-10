//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

#include "../src/validate.h"
#include "../src/graph_val.h"
using namespace InputValidator;					// we are writing an input validator

int main(int argc, char **argv) {
	InputValidator::init(argc, argv);			// initialize streams, rng and parse arguments

	Integer n = testIn.integer(1, 100000);		// read nodes
	testIn.space();
	Integer m = testIn.integer(1, 200000);		// read vertices
	testIn.newline();

	Graph g(1, n+1);
	for (Integer i = 0; i < m; i++) {			// read edges
		Integer a = testIn.integer(1, n+1);
		testIn.space();
		Integer b = testIn.integer(1, n+1);
		testIn.newline();

		g.addEdge(a, b);
	}
	testIn.eof();

	//check stuff, this can also be done on the fly!
	if (g.hasSelfloop()) juryOut << "selfloop: " << g.hasSelfloop().value() << WA;
	if (g.hasMultiedge()) juryOut << "multiedge: " << g.hasMultiedge().value() << WA;
	if (!g.isConnected()) juryOut << "not connected!" << WA;
	if (!g.isTree()) juryOut << "not a tree!" << WA;
	if (!g.isBipartite()) juryOut << "WTF, trees are always bipartite?!" << FAIL;

	juryOut << "Yay!" << AC;
}
