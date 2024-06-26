//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

#include "../src/validate.h"
using namespace Interactor;							// we are writing a           interactor
using namespace Multipass;							//                  multipass

int main(int argc, char **argv) {
	Interactor::init(argc, argv);					// initialize streams, rng and parse arguments
	Multipass::init();

	if (pass == 0) {								// check pass
		Integer n = testIn.integer();				// get the testdata
		toTeam << n << std::endl;					// print to team

		Integer ans = fromTeam.integer();			// get team response
		nextstate << ans << std::endl;				// store ans for next run
		nextpass << n << NEXT;						// write nextpass.in and exit
	} else {
		Integer n = testIn.integer();				// get the testdata (writte previously in nextpass.in)
		toTeam << n << std::endl;					// print to team

		Integer curans = fromTeam.integer();		// get team response
		Integer lastans = prevstate.integer();		// get previous team response

		if (curans == lastans) {
			teamOut << "OK" << AC;
		} else {
			juryOut << "non deterministic" << std::endl;
			teamOut << "wrong answer" << WA;
		}
	}
}
