//============================================================================//
// validate.h example usage                                                   //
//============================================================================//
// Note BAPCtools also invokes the outputValidator in a strict mode i.e. case
// and space sensitive to validate the jury answer. To make this not fail you
// need to correctly validate the whitespaces!

#include "../src/validate.h"

bool verify(Integer n) {
	return n == 6;
}

int main(int argc, char **argv) {
	OutputValidator::init(argc, argv);					// initialize streams, rng and parse arguments
	using namespace OutputValidator;

	std::regex options = caseSensitive.regex("Yes|No");	// regex to check possible answers
	std::string j = juryAns.string(options);			// read jury answer
	std::string t = teamAns.string(options);			// read team answer
	teamAns.newline();									// only relevant when validating the jury answer

	if (stringEqual(j, "Yes")) {						// if the answer is yes
		Integer n = juryAns.integer();					// read the solution
		if (!verify(n)) {								// and verify it (should always be correct!)
			juryOut << "jury solution is invalid!" << FAIL;
		}
	}

	if (stringEqual(t, "Yes")) {						// if the answer is yes verify the solution
		Integer n = juryAns.integer();					// read the solution
		teamAns.newline();								// maybe check whitespaces
		if (!verify(n)) {								// and verify it (could be wrong)
			juryOut << "team solution is invalid!" << WA;
		}
	}
	teamAns.eof();										// check for garbage

	if (stringEqual(j, t)) {							// check if the answers are equal
		juryOut << "OK same answer" << AC;
	} else if (stringEqual(t, "Yes")) {					// and check that team is not better than jury
		juryOut << "team found solution but jury not!" << FAIL;
	} else if (stringEqual(j, "Yes")) {
		juryOut << "jury found solution but team not!" << WA;
	} else {
		juryOut << "how did this happen?!" << FAIL;
	}
}
