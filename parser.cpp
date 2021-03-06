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
	indent = 0;
	std::cout << std::endl << "Parsing..." << std::endl;
	currentToken = *Tokens.begin();
}
struct TreeNode * Parser::Parse()
{
	return program();
}
struct Token Parser::nextToken()
{
	if(tokenIndex != Tokens.end())
	{
		return *(tokenIndex++);
	}
	struct Token NON;
	NON.type = NONTOKEN;
	NON.str = "NON";
	return NON;
}
struct TreeNode * Parser::program()
{
	if (DEBUG)
		std::cout << "-> Program called with Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
    struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->lineNumber = 0;
	node->nValue = -1;
	node->sValue = "Program";
	node->nodeType = UNKNOWN;
	node->typeSpecifier = UNKNOWN;
	node->rename = "NULL";
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = declaration_list();
	Print(node,"",indent);
	return node;
}
struct TreeNode * Parser::declaration_list()
{
	if (DEBUG)
		std::cout << "-> Declaration List called with Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	indent++;
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
	indent--;
	return node;
}
struct TreeNode * Parser::declaration()
{
	indent++;
	if (DEBUG)
		std::cout << "-> Declaration called with Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;

	if(currentToken.type == NONTOKEN)
	return NULL;
	if (DEBUG)
		std::cout << "-> Declaration continued with Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	struct Token typeSpec = currentToken = nextToken();
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->lineNumber = currentToken.line;
	node->nValue = -1;
	node->sValue = "Declaration";
	node->nodeType = DEC;
	node->typeSpecifier = DEC;
	node->rename = "NULL";
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	
	node->lineNumber = typeSpec.line;
	if (typeSpec.str == "void")
		node->typeSpecifier = VOID;
	else if (typeSpec.str == "int")
		node->typeSpecifier = INT;
	else
		return NULL;
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
				std::cout << std::endl;
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
			node->lineNumber = currentToken.line;
			node->nValue = 0;
			node->sValue = "NULL";
			node->rename = "NULL";
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
	node->sibling = declaration_list();
	Print(node, "Sibling",indent);
	indent--;
	return node;
}
struct TreeNode * Parser::param_list()
{
	indent++;
	if (DEBUG)
		std::cout << "-> Parameter List called with Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->lineNumber = currentToken.line;
	node->nValue = -1;
	node->sValue = "Parameter List";
	node->nodeType = PAR;
	node->typeSpecifier = PAR;
	node->rename = "NULL";
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
	Print(node, "C1",indent);
	indent--;
	return node;
}//c1
struct TreeNode * Parser::param()
{
	indent++;
	if (DEBUG)
		std::cout << "-> Paramter called with Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;

	struct Token typeSpec = currentToken = nextToken();
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
			node->nodeType = EMP;
			node->lineNumber = currentToken.line;
			node->nValue = 0;
			node->sValue = "NULL";
			node->rename = "NULL";
			return NULL;
			break;
		default:
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,next.line);
			std::cout << "\"" << next.str << "\"" << " Unexpected token." << std::endl;
			exit(-1);
			break;
	}
	Print(node, "Sibling",indent);
	indent--;
	return node;
}
struct TreeNode * Parser::compound_stmt()
{
	indent++;
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	currentToken = nextToken();
	node->lineNumber = currentToken.line;
	node->nValue = -1;
	node->sValue = "Compound Statement";
	node->nodeType = CST;
	node->typeSpecifier = CST;
	node->rename = "NULL";
	
	if (DEBUG)
		std::cout << "-> Compound Statment received Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
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
	if(currentToken.str[0] != '}')
	{
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
		std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \"}\" is missing." << std::endl;
		exit(-1);
	}
	Print(node, "C2",indent);
	indent--;
	return node;
}
struct TreeNode * Parser::local_declaration()
{
	indent++;
	if (DEBUG)
		std::cout << "-> local_declaration received Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	struct Token typeSpec = currentToken = nextToken();
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->nodeType = DEC;
	node->lineNumber = currentToken.line;
	node->nValue = 0;
	node->sValue = "Local Declearation";
	node->rename = "NULL";
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	if (DEBUG)
		std::cout << typeSpec.str << std::endl;
	if(typeSpec.type != KEYWORDS)
	{
		tokenIndex--;
		indent--;
		return NULL;
	}
	node->lineNumber = typeSpec.line;
	if(typeSpec.str == "void")
		node->typeSpecifier = VOID;
	else if (typeSpec.str == "int")
		node->typeSpecifier = INT;
	else
	{
		indent--;
		return NULL;
	}
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
	Print(node, "Sibling",indent);
	indent--;
	return node;
}
struct TreeNode * Parser::statement_list()
{
	indent++;
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->lineNumber = currentToken.line;
	node->nValue = -1;
	node->sValue = "Statement List";
	node->nodeType = STM;
	node->typeSpecifier = STM;
	node->rename = "NULL";
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
	Print(node, "Sibling",indent);
	indent--;
	return node;
}
struct TreeNode * Parser::statement()
{
	currentToken = nextToken();
	if(DEBUG)
		std::cout << "-> Statment received Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
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
	else if(currentToken.str[0] == '}' || currentToken.type == NONTOKEN)
	{
		return NULL;
	}
	else
	{
		if (currentToken.str[0] == ';' )
		{
			return statement();
		}
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
	currentToken = nextToken();
	node->c1 = expression();
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
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
	if (DEBUG)
		std::cout << "-> before if 1 Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	currentToken = nextToken();
	if (DEBUG && false)
		std::cout << "-> before if 2 Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	if(currentToken.str[0] != '=' && currentToken.type!= SYMBOLCOMP)// it is the = it returns
	{
		if (DEBUG && false)
			std::cout << "-> after if 1 Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
			tokenIndex--;
			tokenIndex--;
			currentToken = nextToken();
			if (DEBUG && false)
				std::cout << "-> Simple_epressive will be called: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
			return simple_expressive();
	}
	else
	{
		tokenIndex--;
		tokenIndex--;
		currentToken = nextToken();
		if (DEBUG && false)
			std::cout << "-> in else 1 Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;

		if(currentToken.type != LETTER)
		{
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
			std::cout << "\"" << currentToken.str << "\"" << " 'Unknown expression." << std::endl;
			exit(-1);
		}
		struct TreeNode * node = new struct TreeNode;
		Trash.push_back(node);
		if (DEBUG && false)
		{
			tokenIndex--;
			currentToken = nextToken();
			std::cout << "-> Var will be called with Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		}
		node->c1 = var();
		tokenIndex--;
		tokenIndex--;
		currentToken = nextToken();
		if (DEBUG && false)
			std::cout << "-> expression will be called with Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		node->c2 = expression();
		node->c3 = NULL;
		node->sibling = NULL;
		//currentToken = nextToken();
		if (DEBUG && false)
			std::cout << "-> in else 4 Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		if(currentToken.str[0] != ';' && currentToken.str[0] != ')')
		{
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,currentToken.line);
			std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \";\" is missing." << std::endl;
			exit(-1);
		}
		if (DEBUG)
			std::cout << "-> expression done with Token at line " << currentToken.line << ": " << currentToken.str << "." << std::endl << "_____" << std::endl;
		return node;
	}
}
struct TreeNode * Parser::var()
{
	indent++;
	if (DEBUG)
		std::cout << "-> Var is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->lineNumber = currentToken.line;
	node->nValue = 0;
	node->sValue = "Variable";
	node->typeSpecifier = -1;
	node->rename = "tmpVar"+ std::to_string(unique++);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	struct Token id = currentToken;
	node->sValue = id.str;
	struct Token next = currentToken = nextToken();
	struct Token arr_val;
	struct Token closure;
	if (DEBUG)
		std::cout << std::endl << id.str << " " << next.str << " ";
	switch (next.str[0])
	{
	case ';':
	case ',':
	case '+':
	case '-':
	case '*':
	case '/':
	case '<':
	case '>':
	case '!':
	case ')':
	case '(':
	case '}':
	case 'N':
	case '=':
		node->nodeType = VAR;
		node->rename = "tmp" + SSTR(unique++);
		if (DEBUG)
			std::cout << std::endl;
		break;
	case '[':
		node->nodeType = ARR;
		node->rename = "tmp" + SSTR(unique++);
		arr_val = currentToken = nextToken();
		if (DEBUG)
			std::cout << arr_val.str << " ";
		if (arr_val.str[0] != ']')
		{
			node->nValue = atoi((const char *)arr_val.str.c_str());
			closure = currentToken = nextToken();
			if (DEBUG)
				std::cout << closure.str << " ";
			if (closure.str[0] != ']')
			{
				printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET, closure.line);
				std::cout << "\"" << closure.str << "\"" << " Unexpected token. did you mean \"]\"?" << std::endl;
				exit(-1);
			}
			else
			{
				currentToken = nextToken();
			}
		}
		break;
	default:
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET, next.line);
		std::cout << "\"" << next.str << "\"" << " Unexpected token." << std::endl;
		exit(-1);
		break;
	}
	if (DEBUG)
		std::cout << "-> Var returned with Token at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	Print(node, "C1",indent);
	indent--;
	return node;
}
struct TreeNode * Parser::simple_expressive()
{
	if(DEBUG)
		std::cout << "-> Simple expression is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;	
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = additiveExpression();
	node->c2 = relop();
	node->c3 = additiveExpression();
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::additiveExpression()
{
	indent++;
	if (DEBUG)
		std::cout << "-> Additive expression is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	if (currentToken.str[0] == ';' || currentToken.str[0] == ')'|| currentToken.str[0] == ')' || currentToken.type == SYMBOLCURL ||  currentToken.type == NONTOKEN)
	{
		tokenIndex--;
		tokenIndex--;
		currentToken = nextToken();
		if (currentToken.str[0] != '=' && currentToken.type != SYMBOLCOMP)
		{
			currentToken = nextToken();
			if (DEBUG)
				std::cout << "-> Additive expression is returning with NULL at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
			indent--;
			return NULL;
		}
		else
		{
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET, currentToken.line);
			std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. Unknown expression." << std::endl;
			exit(-1);
		}
	}
	// Additive_expression addop term || term
	if (DEBUG)
		std::cout << "-> Additive expression grabbing tokens at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	currentToken = nextToken();
	if (DEBUG)
		std::cout << "-> first token at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	currentToken = nextToken();
	if (DEBUG)
		std::cout << "-> second token at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	if (currentToken.str[0] == ';' || currentToken.str[0] == ')' || currentToken.str[0] == ',')
	{
		tokenIndex--;
		tokenIndex--;
		tokenIndex--;
		currentToken = nextToken();
		node->c1 = term();
		node->c2 = NULL;
		node->c3 = NULL;
		node->sibling = NULL;
		node->nodeType = EMP;
		node->lineNumber = currentToken.line;
		node->nValue = 0;
		node->sValue = "Additive Expression";
		node->rename = "NULL";
		if (DEBUG)
			std::cout << "-> Additive expression is returning at (; case): " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		indent--;
		return node;
	}
	else
	{
		node->nodeType = EMP;
		node->lineNumber = currentToken.line;
		node->nValue = 0;
		node->sValue = "Additive Expression";
		node->rename = "NULL";
		tokenIndex--;
		tokenIndex--;
		tokenIndex--;
		currentToken = nextToken();
		node->c1 = term();
		node->c2 = addop();
		node->c3 = term();
		node->sibling = NULL;
		if (DEBUG)
			std::cout << "-> Additive expression is returning at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		indent--;
		return node;
	}
}
struct TreeNode * Parser::relop()
{
	if (DEBUG)
		std::cout << "-> Relop is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	if (currentToken.str[0] == ';' || currentToken.str[0] == ','|| currentToken.str[0] == ')'|| currentToken.str[0] == ')' || currentToken.type == SYMBOLCURL ||  currentToken.type == NONTOKEN || currentToken.type == SYMBOLCOMP)
	{
		if (DEBUG)
			std::cout << "-> Relop is returning with NULL at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		return NULL;
	}
	currentToken = nextToken();
	struct TreeNode * node = new struct TreeNode;
	node->nodeType = EMP;
	node->lineNumber = currentToken.line;
	node->nValue = 0;
	node->sValue = "Relop";
	node->rename = "NULL";
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::addop()
{
	if (DEBUG)
		std::cout << "-> Addop is returning at Token: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	//if (currentToken.str[0] == ';' || currentToken.type == SYMBOLCURL ||  currentToken.type == NONTOKEN)
	if (currentToken.str[0] == ';' || currentToken.str[0] == ','|| currentToken.str[0] == ')'|| currentToken.str[0] == ')' || currentToken.type == SYMBOLCURL ||  currentToken.type == NONTOKEN || currentToken.type == SYMBOLCOMP)
	{
		if (DEBUG)
			std::cout << "-> Addop is returning with NULL at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		return NULL;
	}
	currentToken = nextToken();
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->nodeType = EMP;
	node->lineNumber = currentToken.line;
	node->nValue = 0;
	node->sValue = "Addop";
	node->rename = "NULL";
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::term()
{
	if (DEBUG)
		std::cout << "-> Term is called at Token: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	// factor mulop factor | factor
	if (currentToken.str[0] == ';' || currentToken.str[0] == ')'|| currentToken.str[0] == ')'|| currentToken.str[0] == ')' || currentToken.type == SYMBOLCURL ||  currentToken.type == NONTOKEN)
	{
		if (DEBUG)
			std::cout << "-> Term is returning with NULL at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		return NULL;
	}
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = factor();
	node->c2 = mulop();
	node->c3 = factor();
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::mulop()
{
	if (DEBUG)
		std::cout << "-> Mulop is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	if (currentToken.str[0] == ';' || currentToken.str[0] == ','|| currentToken.str[0] == ')'|| currentToken.str[0] == ')' || currentToken.str[0] == '+' || currentToken.str[0] == '-' || currentToken.type == SYMBOLCURL ||  currentToken.type == NONTOKEN || currentToken.type == SYMBOLCOMP)
	{
		if (DEBUG)
			std::cout << "-> Mulop is returning with NULL at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		return NULL;
	}
	currentToken = nextToken();
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->nodeType = EMP;
	node->lineNumber = currentToken.line;
	node->nValue = 0;
	node->sValue = "Mul op";
	node->rename = "NULL";
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	return node;
}
struct TreeNode * Parser::factor()
{
	indent++;
	if (DEBUG)
		std::cout << "-> factor is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	if (currentToken.str[0] == ';' || currentToken.str[0] == ')'|| currentToken.str[0] == ')' || currentToken.str[0] == '+' || currentToken.str[0] == '-' || currentToken.str[0] == ')' || currentToken.type == NONTOKEN)
	{
		tokenIndex--;
		tokenIndex--;
		currentToken = nextToken();
		if (currentToken.str[0] != '=' && currentToken.type != SYMBOLCOMP)
		{
			currentToken = nextToken();
			if (DEBUG)
				std::cout << "-> factor is returning with NULL at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
			indent--;
			return NULL;
		}
		else
		{
			printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET, currentToken.line);
			std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. Unknown expression." << std::endl;
			exit(-1);
		}
	}
	// (expression) || var || call || NUM
	currentToken = nextToken();
	if (DEBUG)
		std::cout << "-> factor assessing Token: " << currentToken.line << ": " << currentToken.str << ", " << currentToken.type << "." << std::endl;
	if (currentToken.str[0] == '(')
	{
		struct TreeNode * node = new struct TreeNode;
		Trash.push_back(node);
		node->nodeType = EMP;
		node->lineNumber = currentToken.line;
		node->nValue = 0;
		node->sValue = "(Expression)";
		node->rename = "NULL";
		node->c1 = NULL;
		node->c2 = NULL;
		node->c3 = NULL;
		node->sibling = expression();
		currentToken = nextToken();
		if (DEBUG)
			std::cout << "-> factor returned as (expression) Token: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		indent--;
		return node;
	}
	else if (currentToken.type == VAR|| currentToken.str[0] == '[')
	{
		struct TreeNode * node = new struct TreeNode;
		Trash.push_back(node);
		node->nodeType = VAR;
		node->lineNumber = currentToken.line;
		node->nValue = 0;
		node->sValue = "VAR";
		node->rename = "NULL";
		node->c1 = NULL;
		node->c2 = NULL;
		node->c3 = NULL;
		if (currentToken.type == VAR)
		{
			currentToken = nextToken();
			if (currentToken.str[0] == '(')
			{
				struct TreeNode * node = new struct TreeNode;
				Trash.push_back(node);
				node->nodeType = CLL;
				node->lineNumber = currentToken.line;
				node->nValue = 0;
				node->sValue = "Call";
				node->rename = "NULL";
				node->c1 = NULL;
				node->c2 = NULL;
				node->c3 = NULL;
				node->sibling = call();
				if (DEBUG)
					std::cout << "-> factor returned as Call at Token: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
				Print(node, "C1",indent);
				indent--;
				return node;
			}
			else
			{
				tokenIndex--;
				tokenIndex--;
				currentToken = nextToken();
			}
		}
		if (currentToken.str[0] == '[')
		{
			tokenIndex--;
			tokenIndex--;
			currentToken = nextToken();
		}
		node->sibling = var();
		if (DEBUG)
			std::cout << "-> factor returned as var at Token: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		Print(node, "C1",indent);
		indent--;
		return node;
	}
	tokenIndex--;
	tokenIndex--;
	currentToken = nextToken();
	if (currentToken.type == DIGIT)
	{
		struct TreeNode * node = new struct TreeNode;
		Trash.push_back(node);
		node->lineNumber = currentToken.line;
		node->nValue = currentToken.value;
		node->sValue = "Number";
		node->nodeType = NUM;
		node->typeSpecifier = INT;
		node->rename = "tmpVar" + std::to_string(unique++);;
		node->c1 = NULL;
		node->c2 = NULL;
		node->c3 = NULL;
		node->sibling = NULL;
		currentToken = nextToken();
		if (DEBUG)
			std::cout << "-> factor returned as num at Token: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
		Print(node, "C1",indent);
		indent--;
		return node;
	}
}
struct TreeNode * Parser::call()
{
	indent++;
	if (DEBUG)
		std::cout << "-> Call is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	tokenIndex--;
	tokenIndex--;
	currentToken = nextToken();
	node->lineNumber = currentToken.line;
	node->nValue = -1;
	node->sValue = "Call";
	node->nodeType = CLL;
	node->typeSpecifier = -1; // SELECT THIS int VS void ?
	node->rename = "NULL";
	node->c1 = var();
	node->c2 = arg_list();
	node->c3 = NULL;
	node->sibling = NULL;
	if (currentToken.str[0] != ')')
	{
		printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET, currentToken.line);
		std::cout << "\"" << currentToken.str << "\"" << " Unexpected token. \")\" is missing." << std::endl;
		exit(-1);
	}
	Print(node, "Sibling",indent);
	indent--;
	return node;
}
struct TreeNode * Parser::args()
{
	indent++;
	if (DEBUG)
		std::cout << "-> Argument is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	if (currentToken.str[0] == ')')
		return NULL;
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->lineNumber = currentToken.line;
	node->nValue = -1;
	node->sValue = "Argument";
	node->nodeType = ARG;
	node->typeSpecifier = ARG;
	node->rename = "NULL";
	tokenIndex--;
	tokenIndex--;
	currentToken = nextToken();
	node->c1 = expression();
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	if (DEBUG)
		std::cout << "-> Argument is returning at Token: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	Print(node, "Sibling",indent);
	indent--;
	return node;
}
struct TreeNode * Parser::arg_list()
{
	if (DEBUG)
		std::cout << "-> Argument list is raised at: " << currentToken.line << ": " << currentToken.str << "." << std::endl;
	struct TreeNode * node = new struct TreeNode;
	Trash.push_back(node);
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->sibling = NULL;
	struct TreeNode * tmp = node;
	struct TreeNode * tmp2 = args();
	while (tmp2 != NULL)
	{
		tmp->sibling = tmp2;
		tmp = tmp->sibling;
		tmp2 = args();
	}
	return node;
}
void Parser::Print(struct TreeNode * node, std::string title, int depth)
{
	std::string str = "";
	for (int i = 0; i < depth; i++)
	{
		str = str + "   ";
	}
	printf(ANSI_COLOR_CYAN "%s%s\n" ANSI_COLOR_RESET, str.c_str(), title.c_str());
	printf(ANSI_COLOR_YELLOW "%sNode Type:" ANSI_COLOR_RESET, str.c_str());
	switch (node->typeSpecifier)
	{
	case VAR:
		std::cout << "Variable" << std::endl;
		break;
	case ARR:
		std::cout << "Array" << std::endl;
		break;
	case FUN:
		std::cout << "Function" << std::endl;
		break;
	case ARG:
		std::cout << "Arguments" << std::endl;
		break;
	case PAR:
		std::cout << "Parameter List" << std::endl;
		break;
	case CST:
		std::cout << "Compaound statement" << std::endl;
		break;
	case DEC:
		std::cout << "Declaration" << std::endl;
		break;
	case COM:
		std::cout << "Compare" << std::endl;
		break;
	case STM:
		std::cout << "Statement List" << std::endl;
		break;
	case CLL:
		std::cout << "Call" << std::endl;
		break;
	case EMP:
		std::cout << "Empty" << std::endl;
		break;
	default:
		std::cout << "Unknown" << std::endl;
		break;
	}
	printf(ANSI_COLOR_YELLOW "%sLine Number:" ANSI_COLOR_RESET, str.c_str());
	std::cout << node->lineNumber << std::endl;
	printf(ANSI_COLOR_YELLOW "%sName:" ANSI_COLOR_RESET, str.c_str());
	std::cout << node->sValue << std::endl;
	printf(ANSI_COLOR_YELLOW "%sValue:" ANSI_COLOR_RESET, str.c_str());
	std::cout << node->nValue << std::endl;
	printf(ANSI_COLOR_YELLOW "%sData Type:" ANSI_COLOR_RESET, str.c_str());
	switch (node->typeSpecifier)
	{
	case NUM:
	case INT:
		std::cout << "Int" << std::endl;
		break;
	case VOID:
		std::cout << "Void" << std::endl;
		break;
	default:
		std::cout << "Unknown" << std::endl;
		break;
	}
	printf(ANSI_COLOR_YELLOW "%sRename:" ANSI_COLOR_RESET, str.c_str());
	std::cout << node->rename << std::endl;
}
void Parser::PrintSearchTree()
{
	printf(ANSI_COLOR_GREEN "Printing:\n" ANSI_COLOR_RESET);
}
Parser::~Parser()
{
	std::cout << "Destructor called." << std::endl;
	for(std::vector<struct TreeNode *>::iterator it = Trash.begin(); it != Trash.end(); ++it )
	{
		delete (*it);
	}
}

