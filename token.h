#ifndef TOKEN_H
#define TOKEN_H
#include <string>

struct Token{
	int type;
	int value;
	int line;
	std::string text;
};

#endif
