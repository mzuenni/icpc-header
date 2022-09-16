//============================================================================//
// validate.h example usage                                                   //
//============================================================================//
// Note BAPCtools also invokes the outputValidator in a strict mode i.e. case
// and space sensitive to validate the jury answer. To make this not fail you
// need to correctly validate the whitespaces!

#include "../src/validate.h"

int main(int argc, char **argv) {
	OutputValidator::init(argc, argv);					// initialize streams, rng and parse arguments
	using namespace OutputValidator;

	if (stringEqual(juryAns.string(), "impossible")) {	// check if the test has an answer
		teamAns.expectString("impossible");				// check if the participant found no answer either
		teamAns.newline();								// only relevant when validating the jury answer
		teamAns.eof();									// check for garbage
		juryOut << "OK impossible" << AC;
	} else {
		Integer n = testIn.integer();					// get the testdata
		Integer ans = teamAns.integer();				// get team answer
		teamAns.newline();                              // only relevant when validating the jury answer
		if (2*ans != n) juryOut << "wrong answer" << WA;
		teamAns.eof();                                  // check for garbage
		juryOut << "OK possible" << AC;
	}
}
