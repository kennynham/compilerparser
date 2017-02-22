/*
* @author: Kenny Nham, Andrew Ruppel
* @email: drewruppel@csu.fullerton.edu, knham1@csu.fullerton.edu
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <fstream>
#include "lex.h"
#include "token.h"

using namespace std;

queue<Token> Lex::getTokens(){ // Added by Kenny
	return tokenQueue;
}


void Lex::run() {
	int lineNum = 0;
	Token token;

	/*
	* __ FILE INPUT __
	*
	* change input file stream here.
	*/
	string fileName = "input2.txt";

	ifstream inFile(fileName);
	string line;

	if (inFile && inFile.is_open()) {
		cout << "\n\nBeginning Lexer Anaylsis.\n\n";
		while (getline(inFile, line)) {
			lineNum++;
			while (line.length() != 0) {
				// the token lexeme is subtracted from
				// the line and the line picks up at new heading.
				token = nextToken(line);
				
				// if a whitespace is returned 
				if (token.hasNone) {
					line = "";
				} else {
					token.lineNumber = lineNum;
					token.print();
					tokenQueue.push(token); // Added by Kenny
				}

			}
		}

	} else {
		throwErrorAndExit("ERROR: Could not open input file.\n");
	}

	// check for eof
	if (inFile.eof()) {
		token.id = 0;
		token.lexeme = "";
		token.lineNumber = lineNum;
		tokenQueue.push(token);
	}

	cout << "\n\nCompleting Lexer Analysis.\n\n";
	inFile.close();
}

/*
	Grabs a single token from a string

	@param string - the line to be iterated through for tokens
	@return token - object containing the id and lexeme
*/
Token Lex::nextToken(string &line) {
	Token token;
	char currChar;
	char nextChar;
	int column = -1;
	int state = 1;
	int previousState = 1;
	bool backedUp = false;

	for (unsigned int cp = 0; cp < line.length(); cp++) {
		// char pointers
		currChar = line[cp];
		nextChar = line[cp + 1];

		// if comment exit and get new line from file
		if (isComment(currChar, nextChar)) {
			token.hasNone = true;
			return token;
		}

		// set token start if not set, and not whitespace
		//
		// @issue: whitespace
		// issue here was that there was whitespace char
		// after tokenizing our first word. making it exit
		// to a new line. Added check at loop exit.
		if (token.startingLine == -1) {
			if (!iswspace(currChar)) {
				token.startingLine = cp; // set to char pointer
			}
		}

		// get our states
		column = getPosition(currChar);
		previousState = state;
		state = getNextState(state, column);


		// perform lookahead
		if (state == 0) {
			state = doLookahead(previousState, currChar, nextChar);
			if (isAccepted(state)) {
				cp--;
				backedUp = true;
			}
		}

		// check if a token has been found
		if (isAccepted(state)) {
			if (needsBackup(state) && !backedUp) {
				cp--;
			}
			backedUp = false;

			// set/reset token items
			set_token(token, line, cp, state);

			// if token is an identifier
			if (token.id == 2) {
				if (getKeyword(token.lexeme)) {
					token.id = getKeywordID(token.lexeme);
				}
			}

			// remove cp+1 number of spaces from
			// the line so we can start at first char
			// of the next token
			line.replace(0, cp + 1, "");

			// check to see if the new first char of line
			// is a whitespac char, if so let's remove it
			if (iswspace(line[0])) {
				line.replace(0, 1, "");
			}

			return token;
		}
	}
}

bool Lex::isComment(char currChar, char nextChar) {
	if (currChar == '/' && (currChar == nextChar)) {
		return true;
	}
	return false;
}

void Lex::set_token(Token &token, string line, int cp, int state) {
	token.lexeme = line.substr(token.startingLine, cp - token.startingLine + 1);
	token.id = getTokenID(state);
	token.startingLine = -1;
}

int Lex::getPosition(char c) {
	if (isalpha(c) || c == '_') return 1;
	if (isdigit(c)) return 2;
	if (isspace(c)) return 23;
	switch (c) {
	case ',':
		return 3;
		break;
	case ';':
		return 4;
		break;
	case '<':
		return 5;
		break;
	case '>':
		return 6;
		break;
	case '{':
		return 7;
		break;
	case '}':
		return 8;
		break;
	case '[':
		return 9;
		break;
	case ']':
		return 10;
		break;
	case '(':
		return 11;
		break;
	case ')':
		return 12;
		break;
	case '*':
		return 13;
		break;
	case '^':
		return 14;
		break;
	case ':':
		return 15;
		break;
	case '.':
		return 16;
		break;
	case '=':
		return 17;
		break;
	case '-':
		return 18;
		break;
	case '+':
		return 19;
		break;
	case '/':
		return 20;
		break;
	case '"':
		return 21;
		break;
	case '!':
		return 22;
		break;
	default:
		return 24;
	}
	throwErrorAndExit("Error for char: " + c);  
		// Return state "error". This should never be executed since the
		// switch statement has a default case
}

/**
* @param state
* @param column
*
* @return int
*/
int Lex::getNextState(int state, int column) {
	return state_table[state][column];
}


/**
* @param lexeme
*
* @return boolean
*/
bool Lex::getKeyword(string lexeme) {
	if (key_word[lexeme] != 0) {
		return true;
	}
	return false;
}

/**
* @param state
*
* @return boolean
*/
bool Lex::isAccepted(int state) {
	if (state_table[state][25] != -1) {
		return true;
	}
	return false;
}

/**
* @param state
*
* @return boolean
*/
bool Lex::needsBackup(int state) {
	if (state_table[state][25] == 1) {
		return true;
	}
	return false;
}

/**
* @param keyword
*
* @return boolean
*/
int Lex::getKeywordID(string keyword) {
	if (key_word[keyword] != 0) {
		return key_word[keyword];
	}
	return 3;
}

/**
* @param previous_state
* @param current_char
* @param next_char
*
* @return int
*/
int Lex::doLookahead(int previous_state, char current_char, char next_char) {
	switch (previous_state) {
		// if previous is int
		case 4:
			// and next is floaty
			if (current_char == '.') {
				if (isdigit(next_char)) {
					// return float
					return 6; 
				} else {
					// else int
					return 5; 
				}
			}
			break;
		default: 
			return 39;
	}
	// default return
	return 39;
}

/**
* @param state the state's id
*
* @return int
*/
int Lex::getTokenID(int state) {
	return state_table[state][0];
}

/**
* @param string message
*
* Throws error nessage, pauses, and exits program.
*/
void Lex::throwErrorAndExit(string message) {
	cout << "LEXER ERROR:\n";
	cout << message;
	system("PAUSE");
	exit(EXIT_FAILURE);
}

// Holds our key words in an 'associative array' 
// wannabe for each checking
Lex::Lex() {
	key_word = {
		{ "prog", 10 }, { "main", 11 },
		{ "fcn", 12 },{ "class", 13 },
		{ "float", 15 },{ "int", 16 },
		{ "string", 17 },{ "if", 18 },
		{ "elsif", 19 },{ "else", 20 },
		{ "while", 21 },{ "input", 22 },
		{ "print", 23 },{ "new", 24 },
		{ "return", 25 }
	};
}