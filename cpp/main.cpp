/*
* @author: Kenny Nham, Andrew Ruppel
* @email: drewruppel@csu.fullerton.edu, knham1@csu.fullerton.edu
*/
#include "lex.h"
#include "parser.h"

int main() {
	cout << "Begin frontend anaylsis.";

	// Initialize constructors for our front end.
	Lex lexer;
	Parser parser;
	
	lexer.run();
	parser.run(lexer.getTokens());

	cout << "End frontend anaylsis.";
	return 0;
}