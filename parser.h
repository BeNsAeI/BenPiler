#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "token.h"

#define VOID 1
#define INT 2
#define NUM 3
#define VAR 1 // Variable
#define ARR 2 // Array
#define FUN 3 // Function
#define ARG 4 // Arguments
#define PAR 5 // Parameter List
#define CST 6 // Compaound statement
#define DEC 7 // Declaration
#define COM 8 // Compare
#define STM 9 // Statement List
#define CLL 10 // Call
#define EMP 11 // Empty
#define UNKNOWN -1

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
	struct TreeNode * Parse();
	void PrintSearchTree();
	~Parser();
private:
	bool DEBUG;
	unsigned int unique;
	int indent;
	std::vector<struct TreeNode *> Trash;
	std::vector<struct Token> Tokens;
	struct Token nextToken();
	std::vector<struct Token>::iterator tokenIndex;
	struct Token currentToken;
	int currentType,currentLine;
	int indent;
	void Print(struct TreeNode * node, std::string title, int depth);
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
	struct TreeNode * selection_stmt();
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
