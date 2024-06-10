//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

#include "../src/validate.h"
using namespace AnswerValidator;						// we are writing an answer validator

int main(int argc, char **argv) {
	AnswerValidator::init(argc, argv);					// initialize streams, rng and parse arguments

	Integer t = testIn.integer();						// read the number of testcases

	ans.integers(0, 100, t, NEWLINE);					// for each testcase there should be one answer separated by newline
	ans.newline();										// should also end with newline
	ans.eof();											// there should be nothing else
	juryOut << "OK possible" << AC;
}
