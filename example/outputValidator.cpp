//============================================================================//
// validate.h example usage                                                   //
//============================================================================//
// Note BAPCtools also invokes the outputValidator in a strict mode i.e. case
// and space sensitive to validate the jury answer. To make this not fail you
// need to correctly validate the whitespaces!
// Therefore, it is best practice to use the same method to parse team and jury
// input. This has the added benefit that errors in the jury answer can be
// noticed and reported with FAIL (instead of the team silently gettting WA).

#include "../src/validate.h"
using namespace OutputValidator;							// we are writing an output validator

bool verify(Integer n) {
	return n == 6;
}

// same method to parse/verify team and jury input
std::string parseAndVerify(InputStream& in, std::string_view who, Verdict onError) {
	std::regex options = caseSensitive.regex("Yes|No");		// regex to check possible answers
	std::string ans = in.string(options);					// read answer
	teamAns.newline();										// check whitespaces

	if (stringEqual(ans, "Yes")) {							// if the answer is yes
		Integer n = in.integer();							// read the solution
		if (!verify(n)) {									// and verify it
			juryOut << who << " solution is invalid!" << onError;
		}
		in.newline();										// check whitespaces
	}
	in.eof();												// check for garbage
	return ans;
}

int main(int argc, char **argv) {
	OutputValidator::init(argc, argv);						// initialize streams, rng and parse arguments


	std::string j = parseAndVerify(juryAns, "jury", FAIL);	// read jury answer (errors result in FAIL)
	std::string t = parseAndVerify(teamAns, "team", WA);	// read team answer (errors result in WA)

	if (stringEqual(j, t)) {								// check if the answers are equal
		teamOut << "OK same answer" << AC;
	} else if (stringEqual(t, "Yes")) {						// and check that team is not better than jury
		juryOut << "team found solution but jury not!" << FAIL;
	} else if (stringEqual(j, "Yes")) {
		juryOut << "team did not print 6?!" << std::endl;
		teamOut << "jury found solution but team not!" << WA;
	} else {
		juryOut << "how did this happen?!" << FAIL;
	}
	return AC;
}
