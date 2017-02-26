#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <stdio.h>
#include <vector>
#include <regex>

#include "token.h"

class Scanner
{
public:
	Scanner(std::string path,bool debug);
	std::vector<struct Token> Tokens;
	~Scanner();
private:
	bool DEBUG;
	int isToken(std::string str);
	void tokenize();
	void readFile();
	char getLetter();
	int checkpoint; //checkpoint for lkast safe spot for tokenning
	char * buffer;
	std::string file;
	int index;
	int lineIndex;
	FILE *fin;
protected:

};

#endif
