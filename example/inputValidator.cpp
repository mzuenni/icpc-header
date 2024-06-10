//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

// even though its c++17 gcc10 does not support the function std::from_chars
// completely therefore its usage is optional but preferred
//#define DOUBLE_FALLBACK

#include "../src/validate.h"
using namespace InputValidator;					// we are writing an input validator

int main(int argc, char **argv) {
	InputValidator::init(argc, argv);			// initialize streams, rng and parse arguments

	std::string token;

	token = testIn.string();					// read the next token
	testIn.space();								// ensure that tokens a separated by space
	token = testIn.string(5, 7);				// read the next token with length in [5,7)
	testIn.newline();							// ensure newline after second token
	testIn.integer(5, 7);						// read an intger in [5,7)
	testIn.newline();
	testIn.eof();								// read end of file

	// more parse mathods available in InputStream

	// write a message as feedback
	if (false) juryOut << "Oh no!" << WA;
	else juryOut << "Yay!" << AC;

	// this is never reached since we printed an AC/WA verdict which exits the program!
	return 0;
}
