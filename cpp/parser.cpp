/*
* Parser definitions and main functions.
* 
* @author: Kenny Nham, Andrew Ruppel
* @email: drewruppel@csu.fullerton.edu, knham1@csu.fullerton.edu
*/

#include "parser.h"
#include <fstream>

using namespace std;

/*
* __ MAIN PARSER FUNCTION __
*
* @param tokenQueue
* @return none
*/
void Parser::run(queue<Token> tokenQueue) {
	cout << "\n\nBeginning Parser Analysis.\n\n";

	// verify token queue is not empty
	//queue<Token> tokenQueue = tokens;
	checkTokenQueue(tokenQueue);

	Symbol ss;
	Symbol endMarker;
	endMarker.symbolName = "$";
	ss.symbolName = "Pgm";
	
	// Push the end marker and start symbol on the stack.
	pushSymbolToTop(endMarker, tokenQueue);
	pushSymbolToTop(ss, tokenQueue);

	while (!symbolStack.empty()) {
		// get symbol from top of stack
		// get the nexttoken from the tokenQueue
		Symbol topOfStack = symbolStack.back();
		Token nextToken = tokenQueue.front();

		// Check to see if the topOfStack item is terminal
		if (topOfStack.isTerminal()) {
			// if the topOfStack and nextToken are equal, pop
			if (topOfStack.equals(symbolTokenID, nextToken)) {
				tokenQueue.pop();
				popFromTheTop(tokenQueue);
			} else {
				// throw error if terminal and not matching.
				symbolError();
			}
		} else {
			string production = symbolTable[rowNum[topOfStack.symbolName]][columnNum[nextToken.id]];
			//if Table[top of stack, incoming token] is nonblank
			if (production != "") {
				// read in our production string and iterate through each substring tokens.
				// get symbols pertaining to substring tokens.
				istringstream ss(production);
				istream_iterator<string> begin(ss), end;
				deque<Symbol> tempQueue;
				vector<string> symbol_names(begin, end);

				for (int i = 0; i < symbol_names.size(); i++) {
					Symbol temp;
					temp.symbolName = symbol_names[i];
					tempQueue.push_back(temp);
				}
				//pop top of stack and print
				symbolStack.pop_back();
				printStackTrace(tokenQueue);

				//push table[top of stack, incoming token] in reverse order
				while (!tempQueue.empty() && tempQueue.back().symbolName != "eps") {
					symbolStack.push_back(tempQueue.back());
					tempQueue.pop_back();
					printStackTrace(tokenQueue);
				}
			} else {
				// else error
				symbolError();
			}
		}
	}
	system("PAUSE");
}

/*  __DEFAULT PARSER CONSTRUCTOR AND TABLE DEFINITIONS__  */

Parser::Parser()
{
	/*  __TABLE INSTANTIATION__  */

	rowNum = {
		{ "Pgm", 0 },{ "Slist", 1 },{ "Stmt", 2 },{ "S_Out", 3 },
		{ "Y1", 4 },{ "Elist", 5 },{ "Elist2", 6 },{ "E", 7 },
		{ "X1", 8 },{ "T", 9 },{ "X2", 10 },{ "F", 11 },
		{ "Fatom", 12 },{ "Opadd", 13 },{ "Opmul", 14 }
	};

	columnNum = {
		{ 10, 0 },{ 23, 1 },{ 22, 2 },{ 2, 3 },
		{ 3, 4 },{ 4, 5 },{ 5, 6 },{ 37, 7 },
		{ 38, 8 },{ 6, 9 },{ 46, 10 },{ 47, 11 },
		{ 41, 12 },{ 48, 13 },{ 42, 14 },{ 7, 15 },
		{ 33, 16 },{ 34, 17 },{ 0, 18 }
	};

	symbolTokenID = {
		{ "$", 0 },{ "id", 2 },{ "int", 3 },{ "float",4 },{ "string", 5 },
		{"comma", 6},{ "semi", 7 },{ "kwdprog", 10 },{ "kwdinput", 22 },
		{ "kwdprint", 23 },{ "brace1",33 },{ "brace2", 34 },{ "paren1", 37 },
		{ "paren2", 38 },{ "aster", 41 },{ "caret", 42 },{ "equal", 45 },
		{ "minus", 46 },{ "plus", 47 },{ "slash", 48 }
	};

	abbreviatedSymbol = {
		{ "E", "E" },{ "Elist", "EL." },{ "Elist2", "E2." },{ "F", "F." },
		{ "Fatom", "Fa." },{ "Opadd", "O+." },{ "Opmul", "O*." },{ "Pgm", "Pg." },
		{ "S_Asgn", "S=." },{ "S_In", "SI." },{ "S_Out", "SO." },{ "Slist", "SL." },
		{ "Stmt", "S." },{ "Stmts", "Ss." },{ "T", "T." },{ "X1", "X1." },
		{ "X2", "X2." },{ "Y1", "Y1." },{ "aster", "*." },{ "brace1", "{." },
		{ "brace2", "}." },{ "caret", "^." },{ "comma", ",." },{ "equal", "=." },
		{ "float", "fl." },{ "id", "id." },{ "int", "it." },{ "kwdinput", "ki." },
		{ "kwdprint", "ko." },{ "kwdprog", "kp." },{ "minus", "-." },{ "paren1", "(." },
		{ "paren2", ")." },{ "plus", "+." },{ "semi", ";." },{ "slash", "/." },
		{ "string", "st." },{ "$", "$" },{ "eps", "" }

	};
}



/*  __TABLE AND SYMBOL OPS__  */

/**
* Check if symbol starts with lowercase or is $.
*	Lowercase indicates terminal symbol.
*	$ indicates end marker.
* 
* @return bool
*/
bool Parser::Symbol::isTerminal() {
	if ((islower(symbolName[0])) || (symbolName[0] == '$')) {
		return true;
	} else {
		return false;
	}
}

/*
* Check to see if the symbolToken id is equal to the nextToken id.
*
* @param symbolTokenID
* @param nextToken
*
* @return bool
*/
bool Parser::Symbol::equals(map<string, int> symbolTokenID, Token nextToken) {
	return (symbolTokenID[symbolName] == nextToken.id);
}
/**
* Pushes a symbol to the top of the stack.
*
* @param Symbol
* @return none
*/
void Parser::pushSymbolToTop(Symbol symbol, queue<Token> tokenQueue) {
	symbolStack.push_back(symbol);
	printStackTrace(tokenQueue);
}

/**
* Pops a symbol from the top of the stack.
*
* @param tokenQueue
* @return none
*/
void Parser::popFromTheTop(queue<Token> tokenQueue) {
	symbolStack.pop_back();
	printStackTrace(tokenQueue);
}

/**
*	"have the parser output the current stack and 
*	 the current input token each time the parser changes the stack"
*
* @param tokenQueue
* @return none
*/
void Parser::printStackTrace(queue<Token> tokenQueue) {
	ofstream outFile;
	outFile.open("output2.txt", ios_base::app);

	string symbols = "";

	// Iterate our stack trace and print
	deque<Symbol>::const_iterator it;
	for (it = symbolStack.begin(); it != symbolStack.end(); it++) {
		symbols += abbreviatedSymbol[it->symbolName];
	}

	cout << setw(50) << left << symbols;
	outFile << setw(50) << left << symbols;

	// If we still have an element on the front
	// of the tokenQueue let's print it.
	if (!tokenQueue.empty()) {
		cout << setw(20) << tokenQueue.front().lexeme;
		outFile << setw(20) << tokenQueue.front().lexeme;
	}
	
	cout << endl;
	outFile << endl;
	outFile.close();
}

/*
* lookup by value and return the string 'key'
*
* @param symbolTokenID
* @param token
*
* @return string
*/
string Parser::getColumnHeader(map<string, int> symbolTokenID, Token token) {
	// Create our iterator for the symbolToken
	map<string, int>::const_iterator it;

	for (it = symbolTokenID.begin(); it != symbolTokenID.end(); ++it) {
		if (it->second == token.id) {
			return it->first;
		}
	}
}



/*  __ERROR CHECKING AND HANDLING__  */

/**
* Checks to see if the token queue is empty.
* If so throw an error.
*
* @param queue<Token>
* @return none
*/
void Parser::checkTokenQueue(queue<Token> tokenQueue) {
	if (tokenQueue.empty()) {
		throwErrorAndExit("Token Queue is empty.\n Please check input file.\n");
	}
}

/**
* this block will be for stack errors
* The error should indicate the token (i.e., column header) and the top-ofstack
* symbol (i.e., the rule LHS) and the token's line number (if known).
*
* @return none
*/
void Parser::symbolError() {
	cout << "\n\nPARSER ERROR:\n";
	cout << "TOKEN: " << getColumnHeader(symbolTokenID, tokenQueue.front()) << endl;
	cout << "STACK SYMBOL: " << symbolStack.back().symbolName << endl;
	cout << "LINENUMBER: " << (tokenQueue.front().lineNumber != -1) 
								? to_string(tokenQueue.front().lineNumber) : "Unknown";
	throwErrorAndExit("");
}


/**
* Throws error nessage, pauses, and exits program.
*
* @param string
* @return none
*/
void Parser::throwErrorAndExit(string message) {
	cout << "PARSER ERROR:\n";
	cout << message;
	system("PAUSE");
	exit(EXIT_FAILURE);
}

