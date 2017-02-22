/*
* Parser prototypes
* 
* @author: Kenny Nham, Andrew Ruppel
* @email: drewruppel@csu.fullerton.edu, knham1@csu.fullerton.edu
*/

#ifndef PARSER_H
#define PARSER_H

#include "lex.h"

#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>

using namespace std;

class Parser {
private:
	// Symbol struct
	struct Symbol {
		string symbolName;
		bool isTerminal();
		bool equals(map<string, int>, Token);
	};

public:
	// Public member functions
	Parser();
	void run(queue<Token>);
	void checkTokenQueue(queue<Token>);
	void throwErrorAndExit(string);
	void pushSymbolToTop(Symbol, queue<Token>);
	void popFromTheTop(queue<Token>);
	void printStackTrace(queue<Token>);
	void symbolError();
	string getColumnHeader(map<string, int>, Token);

	// Symbol "Stack"
	deque<Symbol> symbolStack;
	queue<Token> tokenQueue;

	// Table Ops
	map<string, int> rowNum;
	map<int, int> columnNum;

	// Symbol Related Tables
	map<string, string> abbreviatedSymbol;
	map<string, int> symbolTokenID;

	string symbolTable[15][19] = {
		//                     0      1      2      3      4      5      6      7      8      9      10     11     12     13     14     15     16     17     18
		//                     prog   print  input  id     int    float  string (      )      ,      -      +      *      /      ^      ;      {      }      $
		/* 0   Pgm     */{ "kwdprog brace1 Slist brace2", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
		/* 1   Slist   */{ "", "Stmt semi Slist", "", "Stmt semi Slist", "", "", "", "", "", "", "", "", "", "", "", "", "", "eps", "" },
		/* 2   Stmt    */{ "", "S_Out", "", "id equal Y1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
		/* 3   S_Out   */{ "", "kwdprint paren1 Elist paren2", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
		/* 4   Y1      */{ "", "", "kwdinput", "E", "E", "E", "E", "E", "", "", "", "", "", "", "", "", "", "", "" },
		/* 5   Elist   */{ "", "", "", "E Elist2", "E Elist2", "E Elist2", "E Elist2", "E Elist2", "eps", "", "", "", "", "", "", "", "", "", "" },
		/* 6   Elist2  */{ "", "", "", "", "", "", "", "", "", "comma Elist", "", "", "", "", "", "", "", "", "" },
		/* 7   E       */{ "", "", "", "T X1", "T X1", "T X1", "T X1", "T X1", "", "", "", "", "", "", "", "", "", "", "" },
		/* 8   X1      */{ "", "", "", "", "", "", "", "", "eps", "eps", "Opadd T X1", "Opadd T X1", "", "", "", "eps", "", "", "" },
		/* 9   T       */{ "", "", "", "F X2", "F X2", "F X2", "F X2", "F X2", "", "", "", "", "", "", "", "", "", "", "" },
		/* 10  X2      */{ "", "", "", "", "", "", "", "", "eps", "eps", "eps", "eps", "Opmul F X2", "Opmul F X2", "Opmul F X2", "eps", "", "", "" },
		/* 11  F       */{ "", "", "", "Fatom", "Fatom", "Fatom", "Fatom", "paren1 E paren2", "", "", "", "", "", "", "", "", "", "", "" },
		/* 12  Fatom   */{ "", "", "", "id", "int", "float", "string", "", "", "", "", "", "", "", "", "", "", "", "" },
		/* 13  Opadd   */{ "", "", "", "", "", "", "", "", "", "", "minus", "plus", "", "", "", "", "", "", "" },
		/* 14  Opmul   */{ "", "", "", "", "", "", "", "", "", "", "", "", "aster", "slash", "caret", "", "", "", "" },
	};
};
#endif
