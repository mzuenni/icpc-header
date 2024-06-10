//============================================================================//
// validate.h example usage                                                   //
//============================================================================//
// Note BAPCtools also invokes the outputValidator in a strict mode i.e. case
// and space sensitive to validate the jury answer. To make this not fail you
// need to correctly validate the whitespaces!

#include "../src/validate.h"
using namespace OutputValidator;						// we are writing an output validator

int main(int argc, char **argv) {
	OutputValidator::init(argc, argv);					// initialize streams, rng and parse arguments

	if (stringEqual(juryAns.string(), "impossible")) {	// check if the test has an answer
		teamAns.expectString("impossible");				// check if the participant found no answer either
		teamAns.newline();								// only relevant when validating the jury answer
		teamAns.eof();									// check for garbage
		teamOut << "OK impossible" << AC;
	} else {
		Integer n = testIn.integer();					// get the testdata
		Integer ans = teamAns.integer();				// get team answer
		teamAns.newline();								// only relevant when validating the jury answer
		if (2*ans != n) {
			juryOut << "should be 2*n" << std::endl;	// a message only the jury can see 
			teamOut << "wrong answer" << WA;			// a message that the team (if enabled for samples) and jury can see
		}
		teamAns.eof();									// check for garbage
		teamOut << "OK possible" << AC;
	}
}
