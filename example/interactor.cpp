//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

#include "../src/validate.h"
using namespace Interactor;							// we are writing an interactor

int main(int argc, char **argv) {
	Interactor::init(argc, argv);					// initialize streams, rng and parse arguments

	Integer n = testIn.integer();					// get the testdata
	toTeam << n << std::endl;						// print to team

	if (n % 2 == 0) {								// check if the test has an answer
		fromTeam.expectString("impossible");		// check that participant did not find an answer either
		fromTeam.newline();							// typically this does nothing since output checking is not space sensitive
		fromTeam.eof();								// check for garbage
		teamOut << "OK" << AC;
	} else {
		Integer ans = fromTeam.integer();			// get team answer
		fromTeam.newline();
		if (2*ans != n) {
			juryOut << "should be 2*n" << std::endl;// a message only the jury can see 
			teamOut << "wrong answer" << WA;		// a message that the team (if enabled for samples) and jury can see
		}
		fromTeam.eof();
		teamOut << "OK" << AC;
	}
}
