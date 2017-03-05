#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "token.h"

#define VOID 1
#define INT 2
#define VAR 1
#define ARR 2
#define FUN 3

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
	struct TreeNode * sibling;
};



class Parser
{
public:
	Parser(std::vector<struct Token> tokens,bool debug);
	struct Tree * getAST();
	~Parser();
private:
	bool DEBUG;
	unsigned int unique;
	std::vector<struct TreeNode *> Trash;
	void match(int expectedType);
	std::vector<struct Token> Tokens;
	struct Token nextToken();
	std::vector<struct Token>::iterator tokenIndex;
	struct Token currentToken;
	int currentType,currentLine;
	struct TreeNode * read_statement();
	struct TreeNode * additiveExpression();
	struct TreeNode * additiveExpression_P();
	struct TreeNode * program();
	struct TreeNode * declaration_list();
	struct TreeNode * declaration_list_P();
	struct TreeNode * declaration();
	struct TreeNode * var_declaration();
	struct TreeNode * type_specifier();
	struct TreeNode * fun_declaration();
	struct TreeNode * params();
	struct TreeNode * param_list();
	struct TreeNode * param_list_P();
	struct TreeNode * param();
	struct TreeNode * compound_stmt();
	struct TreeNode * local_declaration();
	struct TreeNode * local_declaration_P();
	struct TreeNode * statement_list();
	struct TreeNode * statement_list_P();
	struct TreeNode * statement();
	struct TreeNode * expression_stmt();
	struct TreeNode * selection_stmt();
	struct TreeNode * iteration_stmt();
	struct TreeNode * return_stmt();
	struct TreeNode * expression();
	struct TreeNode * var();
	struct TreeNode * simple_expressive();
	struct TreeNode * relop();
	struct TreeNode * addop();
	struct TreeNode * term();
	struct TreeNode * term_P();
	struct TreeNode * mulop();
	struct TreeNode * factor();
	struct TreeNode * call();
	struct TreeNode * args();
	struct TreeNode * arg_list();
	struct TreeNode * arg_list_P();
protected:
};

#endif
