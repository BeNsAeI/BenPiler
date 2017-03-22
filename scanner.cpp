#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <regex>

#include "scanner.h"
#include "token.h"
#include "color.h"

Scanner::Scanner(std::string path,bool debug)
{
	DEBUG = debug;
	char mode = 'r';
	const char * filePath = path.c_str();
	fin = fopen(filePath, &mode);
	if (fin == NULL) {
		printf("Can't open input file\n");
		exit(1);
	}
	if(DEBUG)
	{
		printf(ANSI_COLOR_CYAN "File opened successfully.\n" ANSI_COLOR_RESET);
	}
	readFile();
	tokenize();
}
Scanner::~Scanner()
{
	fclose(fin);
}
void Scanner::tokenize()
{
	checkpoint = 0;
	lineIndex = -2;
	std::string str;
	char current;
	struct Token currentToken;
	while((unsigned int)index < file.size())
	{
		current = getLetter();
		if(current == '\n')
		{
			currentToken.line = ++lineIndex;
			current = ' ';
			if (DEBUG)
				std::cout << "Line is: " << lineIndex << std::endl;
		}
		str = str+current;
		std::string strNext = str + file[index];
		if(str == "/*")
		{
			while(true)
			{
				current = getLetter();
				if (current == '\n')
				{
					lineIndex++;
				}
				if(current == '*')
					if(file[index] == '/')
						break;
			}
			index++;
			str = " ";
		}
		if(isToken(str)!= 0 && isToken(strNext)==0)
		{
			checkpoint = index;
			currentToken.str = str;
			currentToken.type = isToken(str);
			if(currentToken.type != 2)
			{
				currentToken.value = -1;
			}else{
				currentToken.value = atoi( currentToken.str.c_str());
			}
			if(currentToken.type != 20){
				Tokens.push_back(currentToken);
			}
			str ="";
			if(file[index] == ' ' || file[index] == '\n' || file[index] == '\t')
				index++;
			if(DEBUG && currentToken.type != 20)
			{
				printf(ANSI_COLOR_CYAN "Token:" ANSI_COLOR_RESET);
				std::cout << currentToken.type << ", " << currentToken.value << ", " << currentToken.line << ", "<< currentToken.str << std::endl;
			}
		}else{
			if((isToken(str)==0) &&((unsigned int)index < file.size())&&(file[index] == ' ' || file[index] == '\n' || file[index] == '\t'))
			{
				printf(ANSI_COLOR_RED "error " ANSI_COLOR_RESET "at line " ANSI_COLOR_CYAN "%d: " ANSI_COLOR_RESET,lineIndex);
				std::cout << "\"" << str << "\"" << " is not defined." << std::endl;
				str = "";
			}
		}
	}
}
void Scanner::readFile()
{
	ssize_t read;
	size_t len = 0;
	while ((read = getline(&buffer, &len, fin)) != -1)
	{
		if(DEBUG)
		{
			printf(ANSI_COLOR_RED "Recieved: " ANSI_COLOR_RESET);
			std::cout << buffer << std::endl;
		}
		file = file + buffer +'\n';
	}
	if(DEBUG)
	{
		printf(ANSI_COLOR_CYAN "file: \n");
		std::cout << file << std::endl;
		printf(ANSI_COLOR_RESET);
	}
}
char Scanner::getLetter()
{
	char curr = file[index++];
	if(curr == '\t')
		return ' ';
	return curr;
}
int Scanner::isToken(std::string str)
{
	std::regex letter("^[a-zA-Z]+$");
	std::regex digit("^[0-9]+$");
	std::regex symbolPlus("^\\+$");
	std::regex symbolMinus("^\\-$");
	std::regex symbolStar("^\\*$");
	std::regex symbolSlash("^\\/$");
	std::regex symbolComp("^\\<|\\>|\\<\\=|\\>\\=|\\=\\=|\\!\\=$");
	std::regex symbolEqual("^\\=$");
	std::regex symbolSemi("^\\;$");
	std::regex symbolComma("^\\,$");
	std::regex symbolPar("^\\(|\\)$");
	std::regex symbolBra("^\\[|\\]$");
	std::regex symbolCurl("^\\{|\\}$");
	std::regex symbolComment("^\\/\\*|\\*\\/$");
	std::regex keywords("^else|if|int|return|void|while|call|read|write$");
	std::regex read("^read$");
	std::regex write("^write$");

	if(str == " " || str == "\t" ||str == "\n")
		return 20;
	if(regex_match(str,keywords))
		return KEYWORDS;
	if(regex_match(str,letter))
		return LETTER;
	if(regex_match(str,digit))
		return DIGIT;
	if(regex_match(str,symbolPlus))
		return SYMBOLPLUS;
	if(regex_match(str,symbolMinus))
		return SYMBOLMINUS;
	if(regex_match(str,symbolStar))
		return SYMBOLSTAR;
	if(regex_match(str,symbolSlash))
		return SYMBOLSLASH;
	if(regex_match(str,symbolComp))
		return SYMBOLCOMP;
	if(regex_match(str,symbolEqual))
		return SYMBOLEQUAL;
	if(regex_match(str,symbolSemi))
		return SYMBOLSEMI;
	if(regex_match(str,symbolComma))
		return SYMBOLCOMMA;
	if(regex_match(str,symbolPar))
		return SYMBOLPAR;
	if(regex_match(str,symbolBra))
		return SYMBOLBRA;
	if(regex_match(str,symbolCurl))
		return SYMBOLCURL;
	if(regex_match(str,symbolComment))
		return SYMBOLCOMMENT;
	if(regex_match(str,read))
		return READ;
	if(regex_match(str,write))
		return WRITE;
	return 0;
}
