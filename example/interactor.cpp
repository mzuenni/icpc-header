//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

#include "../src/validate.h"

int main(int argc, char **argv) {
	Interactor::init(argc, argv);					// initialize streams, rng and parse arguments
	using namespace Interactor;

	Integer n = testIn.integer();					// get the testdata
	toTeam << n << std::endl;						// print to team

	if (n % 2 == 0) {								// check if the test has an answer
		fromTeam.expectString("impossible");		// check that participant did not find an answer either
		fromTeam.newline();							// typically this does nothing since output checking is not space sensitive
		fromTeam.eof();								// check for garbage
		juryOut << "OK" << AC;
	} else {
		Integer ans = fromTeam.integer();			// get team answer
		fromTeam.newline();
		if (2*ans != n) juryOut << "wrong answer" << WA;
		fromTeam.eof();
		juryOut << "OK" << AC;
	}
}
