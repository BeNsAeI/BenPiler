#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <stdio.h>

#include "token.h"

class Scanner
{
public:
	Scanner(std::string path,bool debug);
	struct Token Tokens[];
	~Scanner();
private:
	bool DEBUG;
	void tokenize();
	void readFile();
	char getLetter();
	char * buffer;
	std::string file;
	int index;
	FILE *fin;
protected:

};

#endif
