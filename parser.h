#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "token.h"

struct TreeNode;

struct TreeNode{
	int lineNumber;
	int nValue;
	std::string sValue;
	int nodeType;
	int typeSpecifier;
	std::string rename;
	bool visited;

	struct TreeNode * c1;
	struct TreeNode * c2;
	struct TreeNode * c3;
	struct TreeNode * sibiling;
};



class Parser
{
public:
	Parser(std::vector<struct Token> tokens);
	void match(int expectedType);
	~Parser();
private:
	std::vector<struct Token> Tokens;
	struct Token nextToken();
	std::vector<struct Token>::iterator tokenIndex;
	struct Token currentToken;
	int currentType,currentLine;
protected:
};

#endif
