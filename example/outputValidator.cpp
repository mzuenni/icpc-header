//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

#include "../src/validate.h"

int main(int argc, char **argv) {
	OutputValidator::init(argc, argv);					// initialize streams, rng and parse arguments
	using namespace OutputValidator;

	if (stringEqual(juryAns.string(), "impossible")) {	// check if the test has an answer
		teamAns.expectString("impossible");				// check if the participant found no answer either
		teamAns.newline();								// typically this does nothing since output checking is not space sensitive
		teamAns.eof();									// check for garbage
		juryOut << "OK" << AC;
	} else {
		Integer n = testIn.integer();					// get the testdata
		Integer ans = teamAns.integer();				// get team answer
		teamAns.newline();
		if (2*ans != n) juryOut << "wrong answer" << WA;
		teamAns.eof();
		juryOut << "OK" << AC;
	}
}
