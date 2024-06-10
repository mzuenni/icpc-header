//============================================================================//
// validate.h example usage                                                   //
//============================================================================//
#include "../src/validate.h"
using namespace InputValidator;									// we are writing an input validator

int main(int argc, char **argv) {
	InputValidator::init(argc, argv);							// initialize streams, rng and parse arguments

	Integer n = testIn.integer(1, 100'000, constraint["n"]);	// read n and store in constraint file
	Integer sum = 0;
	for (Integer i = 0; i < n; i++) {
		sum += testIn.integer(1, 1'000'000, constraint["a"]);	// read a_i and store in constraint file
	}
	constraint["sum"].log(1, 1'000'000'000, "sum");				// store sum over a_i manually in constraint file

	// write a message as feedback
	if (false) juryOut << "Oh no!" << WA;
	else juryOut << "Yay!" << AC;

	// this is never reached since we printed an AC/WA verdict which exits the program!
	return 0;
}
