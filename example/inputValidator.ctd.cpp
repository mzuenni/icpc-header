//============================================================================//
// validate.h example usage                                                   //
//============================================================================//

#include "../src/validate.h"

int main(int argc, char **argv) {
	InputValidator::init(argc, argv);
	using namespace InputValidator;

	std::string token;

	using namespace ctd; // enable ctd syntax

	// same as the other input validator but in a CheckTestData like syntax
	testIn >> STRING(token) >> SPACE >> STRING(5, 7, token) >> NEWLINE;
	testIn >> INT(5, 7) >> NEWLINE;
	testIn >> ENDFILE;

	// more parse mathods available in namespace ctd

	// write a message as feedback
	if (false) juryOut << "Oh no!" << WA;
	else juryOut << "Yay!" << AC;

	// this is never reached since we printed an AC/WA verdict which exits the program!
	return 0;
}
