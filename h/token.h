/*
* @author: Kenny Nham, Andrew Ruppel
* @email: drewruppel@csu.fullerton.edu, knham1@csu.fullerton.edu
*/
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

struct Token {
	int id;
	int startingLine;
	int lineNumber;
	bool hasNone;
	string lexeme;

	Token() {
		id = -1;
		startingLine = -1;
		lineNumber = 0;
		hasNone = false;
		lexeme = "";
	}

	void print() {
		string output = "";

		switch (id) {
			case 3:
				output += " int= " + lexeme;
				break;
			case 4: 
				output += " float= " + lexeme;
				break;
			case 5:
				lexeme = lexeme.substr(1, lexeme.length() - 2);
				break;
		}

		cout << "(Tok: id= " << right << setw(2) << to_string(id) 
						<< " line= " << to_string(lineNumber) 
						<< " str= \"" + lexeme + "\"" << output << ")" << endl;
	}
};
#endif