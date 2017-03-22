#include<stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "color.h"
#include "parser.h"
#include "token.h"

Parser::Parser(std::vector<struct Token> tokens,bool debug)
{
	DEBUG = debug;
	Tokens = tokens;
	tokenIndex = Tokens.begin();
	unique = 0;
	if(DEBUG)
	{
		//currentToken = *(tokenIndex);
		//currentType = tokenIndex->type;
		//match(tokenIndex->type);
		program();
	}
}
struct Token Parser::nextToken()
{
	if(tokenIndex != Tokens.end())
	{
		return *(tokenIndex++);
	}
	struct Token NON;
	NON.type = 0;
	NON.str = "NON";
	return NON;
}
void Parser::match(int expectedType)
{
	if(currentType == expectedType)
	{
		currentToken = currentToken = nextToken();
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
struct TreeNode * Parser::additiveExpression_P()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c1 = NULL;
	node->c1 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::program()
{
      	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->lineNumber = 0;
	node->nValue = -1;
	node->sValue = "Program";
	node->nodeType = -1;
	node->typeSpecifier = -1;
	node->rename = "NULL";
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = declaration_list();
	return node;
}
struct TreeNode * Parser::declaration_list()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	struct TreeNode * tmp = node;
	struct TreeNode * tmp2 = declaration();
	while(tmp2 != NULL)
	{
		tmp->sibling = tmp2;
		tmp = tmp->sibling;
		tmp2 = declaration();
	}
	return node;
}
struct TreeNode * Parser::declaration()
{
	if(currentToken.type == NONTOKEN)
	return NULL;
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	struct Token typeSpec = currentToken = nextToken();
	node->lineNumber = typeSpec.line;
	if(typeSpec.str == "void")
		node->typeSpecifier = VOID;
	else
		node->typeSpecifier = INT;
	struct Token id = currentToken = nextToken();
	node->sValue = id.str;
	struct Token next = currentToken = nextToken();
	struct Token arr_val;
	struct Token closure;
	struct Token semi;
	if(DEBUG)
		std::cout << std::endl << typeSpec.str << " " << id.str << " " << next.str << " ";
	switch(next.str[0])
	{
		case ';':
			node->nodeType = VAR;
			node->rename = "tmp" + SSTR(unique++);
			if(DEBUG)
				std::cout <<std::endl;
			break;
		case '[':
			node->nodeType = ARR;
			node->rename = "tmp" + SSTR(unique++);
			arr_val = currentToken = nextToken();
			if(DEBUG)
				std::cout << arr_val.str << " ";
			if(arr_val.str[0] != ']')
			{
				node->nValue = atoi((const char *)arr_val.str.c_str());
				closure = currentToken = nextToken();
				if(DEBUG)
					std::cout << closure.str << " ";
				if(closure.str[0] != ']')
				{
					printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,closure.line);
					std::cout << "\"" << closure.str << "\"" << " Unexpected token. did you mean \"]\"?" << std::endl;
					exit(-1);
				}
			}
			semi = currentToken = nextToken();
			if(DEBUG)
				std::cout << semi.str;
			if(semi.str[0] != ';')
			{
				printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,semi.line);
				std::cout << "\"" << semi.str << "\"" << " Unexpected token. \";\" is missing." << std::endl;
				exit(-1);
			}
			break;
		case '(':
			node->nodeType = FUN;
			node->c1 = param_list();
			if(DEBUG)
				std::cout << std::endl;
			node->c2 = compound_stmt();
			break;
		default:
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,next.line);
			std::cout << "\"" << next.str << "\"" << " Unexpected token." << std::endl;
			exit(-1);
			break;
	}
	return node;
}
struct TreeNode * Parser::param_list()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	struct TreeNode * tmp = node;
	struct TreeNode * tmp2 = param();
	while(tmp2 != NULL)
	{
		tmp->sibling = tmp2;
		tmp = tmp->sibling;
		tmp2 = param();
	}
	return node;
}
struct TreeNode * Parser::param()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	struct Token typeSpec = currentToken = nextToken();
	node->lineNumber = typeSpec.line;
	if(typeSpec.str == "void")
		node->typeSpecifier = VOID;
	else
		node->typeSpecifier = INT;
	struct Token id = currentToken = nextToken();
	node->sValue = id.str;
	struct Token next = currentToken = nextToken();
	struct Token arr_val;
	struct Token closure;
	struct Token semi;
	if(DEBUG)
		std::cout << typeSpec.str << " " << id.str << " " << next.str << " ";
	switch(next.str[0])
	{
		case ',':
			node->nodeType = VAR;
			node->rename = "tmp" + SSTR(unique++);
			break;
		case '[':
			node->nodeType = ARR;
			node->rename = "tmp" + SSTR(unique++);
			arr_val = currentToken = nextToken();
			if(DEBUG)
				std::cout << arr_val.str << " ";
			if(arr_val.str[0] != ']')
			{
				node->nValue = atoi((const char *)arr_val.str.c_str());
				closure = currentToken = nextToken();
				if(DEBUG)
					std::cout << closure.str << " ";
				if(closure.str[0] != ']')
				{
					printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,closure.line);
					std::cout << "\"" << closure.str << "\"" << " Unexpected token. did you mean \"]\"?" << std::endl;
					exit(-1);
				}
			}
			semi = currentToken = nextToken();
			if(DEBUG)
				std::cout << semi.str;
			if(semi.str[0] != ',' && semi.str[0]!= ')')
			{
				printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,semi.line);
				std::cout << "\"" << semi.str << "\"" << " Unexpected token. \";\" is missing." << std::endl;
				exit(-1);
			}
			break;
		case ')':
			return NULL;
			break;
		default:
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,next.line);
			std::cout << "\"" << next.str << "\"" << " Unexpected token." << std::endl;
			exit(-1);
			break;
	}
	return node;
}
struct TreeNode * Parser::compound_stmt()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	currentToken = nextToken();
	if(currentToken.str[0] != '{')
	{
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
		std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \"{\" is missing." << std::endl;
		exit(-1);
	}
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	struct TreeNode * tmp = node;
	struct TreeNode * tmp2 = local_declaration();
	while(tmp2 != NULL)
	{
		tmp->sibling = tmp2;
		tmp = tmp->sibling;
		tmp2 = local_declaration();
	}
	node->c1 = node->sibling;
	node->sibling = NULL;
	node->c2 = statement_list();
	currentToken = nextToken();
	if(currentToken.str[0] != '}')
	{
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
		std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \"}\" is missing." << std::endl;
		exit(-1);
	}
	return node;
}
struct TreeNode * Parser::local_declaration()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	struct Token typeSpec = currentToken = nextToken();
	std::cout << typeSpec.str << std::endl;
	if(typeSpec.type != KEYWORDS)
	{
		tokenIndex--;
		return NULL;
	}
	node->lineNumber = typeSpec.line;
	if(typeSpec.str == "void")
		node->typeSpecifier = VOID;
	else
		node->typeSpecifier = INT;
	struct Token id = currentToken = nextToken();
	node->sValue = id.str;
	struct Token next = currentToken = nextToken();
	struct Token arr_val;
	struct Token closure;
	struct Token semi;
	if(DEBUG)
		std::cout << std::endl << typeSpec.str << " " << id.str << " " << next.str << " ";
	switch(next.str[0])
	{
		case ';':
			node->nodeType = VAR;
			node->rename = "tmp" + SSTR(unique++);
			if(DEBUG)
				std::cout <<std::endl;
			break;
		case '[':
			node->nodeType = ARR;
			node->rename = "tmp" + SSTR(unique++);
			arr_val = currentToken = nextToken();
			if(DEBUG)
				std::cout << arr_val.str << " ";
			if(arr_val.str[0] != ']')
			{
				node->nValue = atoi((const char *)arr_val.str.c_str());
				closure = currentToken = nextToken();
				if(DEBUG)
					std::cout << closure.str << " ";
				if(closure.str[0] != ']')
				{
					printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,closure.line);
					std::cout << "\"" << closure.str << "\"" << " Unexpected token. did you mean \"]\"?" << std::endl;
					exit(-1);
				}
			}
			semi = currentToken = nextToken();
			if(DEBUG)
				std::cout << semi.str;
			if(semi.str[0] != ';')
			{
				printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,semi.line);
				std::cout << "\"" << semi.str << "\"" << " Unexpected token. \";\" is missing." << std::endl;
				exit(-1);
			}
			break;
		default:
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,next.line);
			std::cout << "\"" << next.str << "\"" << " Unexpected token." << std::endl;
			exit(-1);
			break;
	}
	return node;
}
struct TreeNode * Parser::statement_list()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	struct TreeNode * tmp = node;
	struct TreeNode * tmp2 = statement();
	while(tmp2 != NULL)
	{
		tmp->sibling = tmp2;
		tmp = tmp->sibling;
		tmp2 = statement();
	}
	return node;
}
struct TreeNode * Parser::statement()
{
	currentToken = nextToken();

	if(currentToken.type == KEYWORDS)
	{
		switch(currentToken.str[0])
		{
			case 'i':
				return selection_stmt();
				break;
			case 'w':
				return iteration_stmt();
				break;
			case 'r':
				return return_stmt();
				break;
			case 'c':
				return call();
				break;
		}
	}
	else if(currentToken.str[0] == '{')
	{
		tokenIndex--;
		return compound_stmt();
	}
	else if(currentToken.type == NONTOKEN)
	{
		return NULL;
	}
	else
	{
		if(currentToken.str[0] == ';')
			return statement();
		else
		{
			tokenIndex--;
			return expression();
		}
	}
}
struct TreeNode * Parser::selection_stmt()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	currentToken = nextToken();
	if(currentToken.str[0] != '(')
	{
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
		std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \"(\" is missing." << std::endl;
		exit(-1);
	}
	node->c1 = expression();
	currentToken = nextToken();
	if(currentToken.str[0] != ')')
	{
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
		std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \")\" is missing." << std::endl;
		exit(-1);
	}
	node->c2 = statement();
	currentToken = nextToken();
	if(currentToken.str == "else")
	{
		node->c3 = statement();
	}
	else
	{
		tokenIndex--;
		node->c3 = NULL;
	}
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::iteration_stmt()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	currentToken = nextToken();
	if(currentToken.str[0] != '(')
	{
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
		std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \"(\" is missing." << std::endl;
		exit(-1);
	}
	node->c1 = expression();
	currentToken = nextToken();
	if(currentToken.str[0] != ')')
	{
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
		std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \")\" is missing." << std::endl;
		exit(-1);
	}
	node->c2 = statement();
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::return_stmt()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = expression();
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	currentToken = nextToken();
	if(currentToken.str[0] != ';')
	{
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
		std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \";\" is missing." << std::endl;
		exit(-1);
	}
	return node;
}
struct TreeNode * Parser::expression()
{
	currentToken = nextToken();
	currentToken = nextToken();
	if(currentToken.str[0] != '=')// it is the = it returns
	{
		tokenIndex--;
		tokenIndex--;
		tokenIndex--;
		currentToken = nextToken();
		if (DEBUG)
			std:: << "-> Token at " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		return simple_expressive();
	}
	else
	{
		tokenIndex--;
		tokenIndex--;
		currentToken = nextToken();
		if(currentToken.type != LETTER)
		{
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
			std::cout << "\"" << currentToken.str << "\"" << " 'Unknown expression." << std::endl;
			exit(-1);
		}
		struct TreeNode * node = new struct TreeNode;
		Trash.push_back(node);
		tokenIndex--;
		node->c1 = var();
		currentToken = nextToken();
		node->c2 = expression();
		node->c3 = NULL;
		node->sibling = NULL;
		currentToken = nextToken();
		if(currentToken.str[0] != ';')
		{
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
			std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \";\" is missing." << std::endl;
			exit(-1);
		}
		return node;
	}
}
struct TreeNode * Parser::var()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::simple_expressive()
{
	if(DEBUG)
		std::cout << "->Simple expression is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;	
	struct TreeNode * node = new struct TreeNode;
	// Additive-expr relop Additive-expr; | Additive-expr;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::relop()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::addop()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::term()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::term_P()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c1 = NULL;
	node->c1 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::mulop()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::factor()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::call()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::args()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::arg_list()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::arg_list_P()
{
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c1 = NULL;
	node->c1 = NULL;
	node->sibling = NULL;
	return node;
}
Parser::~Parser()
{
	for(std::vector<struct TreeNode *>::iterator it = Trash.begin(); it != Trash.end(); ++it )
	{
		delete (*it);
		Trash.erase(it);
	}
}

