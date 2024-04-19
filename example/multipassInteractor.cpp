//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

#include "../src/validate.h"

int main(int argc, char **argv) {
	Interactor::init(argc, argv);					// initialize streams, rng and parse arguments
	Multipass::init(argc, argv);
	using namespace Interactor;
	using namespace Multipass;

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
