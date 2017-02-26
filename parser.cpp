#include<stdio.h>
#include <stdlib.h>
#include <iostream>

#include "color.h"
#include "parser.h"
#include "token.h"

Parser::Parser(std::vector<struct Token> tokens,bool debug)
{
	DEBUG = debug;
	Tokens = tokens;
	tokenIndex = Tokens.begin();
	if(DEBUG)
	{
		currentToken = *(tokenIndex);
		currentType = tokenIndex->type;
		match(tokenIndex->type);
	}
}
struct Token Parser::nextToken()
{
	if(tokenIndex != Tokens.end())
	{
		return *(tokenIndex++);
	}
}
void Parser::match(int expectedType)
{
	if(currentType == expectedType)
	{
		currentToken = nextToken();
		std::cout << currentToken.str<<std::endl;
		currentType = currentToken.type;
		currentLine = currentToken.line;
	}else{
		printf(ANSI_COLOR_RED "Fatal error: " ANSI_COLOR_RESET);
		std::cout << "Unexpected token at line ";
		printf(ANSI_COLOR_CYAN "%d\n" ANSI_COLOR_RESET,currentToken.line);
		exit(1);
	}
}
struct TreeNode * Parser::read_statement()
{
	struct TreeNode * myTree = new struct TreeNode;
	myTree->lineNumber = currentLine;
	match(READ);
	//myTree->c1 = variable();
	return myTree;
}
struct TreeNode * Parser::additiveExpression()
{
	TreeNode * t;
	TreeNode *tmp;
	//t = term();
	while(currentType == SYMBOLPLUS || currentType == SYMBOLMINUS)
	{
		tmp = new struct TreeNode;
		tmp->nodeType = currentType;
		tmp->lineNumber = currentLine;
	//	accept();
		tmp->c1 = t;
		t = tmp;
	//	t->c2 = term();
	}
	return t;
}
struct TreeNode * Parser::program()
{
}
struct TreeNode * Parser::declaration_list()
{
}
struct TreeNode * Parser::declaration()
{
}
struct TreeNode * Parser::var_declaration()
{
}
struct TreeNode * Parser::type_specifier()
{
}
struct TreeNode * Parser::fun_declaration()
{
}
struct TreeNode * Parser::params()
{
}
struct TreeNode * Parser::param_list()
{
}
struct TreeNode * Parser::param()
{
}
struct TreeNode * Parser::compound_stmt()
{
}
struct TreeNode * Parser::local_declaration()
{
}
struct TreeNode * Parser::statement_list()
{
}
struct TreeNode * Parser::statement()
{
}
struct TreeNode * Parser::expression_stmt()
{
}
struct TreeNode * Parser::section_stmt()
{
}
struct TreeNode * Parser::iteration_stmt()
{
}
struct TreeNode * Parser::return_stmt()
{
}
struct TreeNode * Parser::expression()
{
}
struct TreeNode * Parser::var()
{
}
struct TreeNode * Parser::simple_expressive()
{
}
struct TreeNode * Parser::relop()
{
}
struct TreeNode * Parser::addop()
{
}
struct TreeNode * Parser::term()
{
}
struct TreeNode * Parser::mulop()
{
}
struct TreeNode * Parser::factor()
{
}
struct TreeNode * Parser::call()
{
}
struct TreeNode * Parser::args()
{
}
struct TreeNode * Parser::arg_list()
{
}
Parser::~Parser()
{

}

