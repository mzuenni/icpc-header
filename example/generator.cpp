//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

#include "../src/validate.h"
using namespace Generator;									// we are writing a genrator

int main(int argc, char **argv) {
	Generator::init(argc, argv);							// initialize streams, rng and parse arguments

	// Integer is a 64bit signed Integer
	// Real is a long double
	Integer n = arguments["--n"].asInteger();				// parse the comandline argument "--n <x>" <x> as integer

	std::vector<Integer> data = Random::distinct(n, 0, 10); // generate n distinct integers in [0, 10)
	// more method available in namespace Random

	testOut << n << std::endl;
	testOut << join(data.begin(), data.end()) << std::endl; // print the numbers;
}
