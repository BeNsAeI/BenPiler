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
	Parser(std::vector<struct Token> tokens,bool debug);
	struct Tree * getAST();
	~Parser();
private:
	bool DEBUG;
	void match(int expectedType);
	std::vector<struct Token> Tokens;
	struct Token nextToken();
	std::vector<struct Token>::iterator tokenIndex;
	struct Token currentToken;
	int currentType,currentLine;
	struct TreeNode * read_statement();
	struct TreeNode * additiveExpression();
	struct TreeNode * program();
	struct TreeNode * declaration_list();
	struct TreeNode * declaration();
	struct TreeNode * var_declaration();
	struct TreeNode * type_specifier();
	struct TreeNode * fun_declaration();
	struct TreeNode * params();
	struct TreeNode * param_list();
	struct TreeNode * param();
	struct TreeNode * compound_stmt();
	struct TreeNode * local_declaration();
	struct TreeNode * statement_list();
	struct TreeNode * statement();
	struct TreeNode * expression_stmt();
	struct TreeNode * section_stmt();
	struct TreeNode * iteration_stmt();
	struct TreeNode * return_stmt();
	struct TreeNode * expression();
	struct TreeNode * var();
	struct TreeNode * simple_expressive();
	struct TreeNode * relop();
	struct TreeNode * addop();
	struct TreeNode * term();
	struct TreeNode * mulop();
	struct TreeNode * factor();
	struct TreeNode * call();
	struct TreeNode * args();
	struct TreeNode * arg_list();
protected:
};

#endif
