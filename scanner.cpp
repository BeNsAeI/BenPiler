#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>

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
	}
	if(DEBUG)
	{
		printf(ANSI_COLOR_CYAN"File opened successfully.\n"ANSI_COLOR_RESET);
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
	
}
void Scanner::readFile()
{
	ssize_t read;
	size_t len = 0;
	while ((read = getline(&buffer, &len, fin)) != -1)
	{
		if(DEBUG)
		{
			printf(ANSI_COLOR_RED"Recieved: "ANSI_COLOR_RESET);
			std::cout << buffer << std::endl;
		}
		file = file + buffer + '\n';
	}
	if(DEBUG)
	{
		printf(ANSI_COLOR_CYAN"file: \n");
		std::cout << file << std::endl;
		printf(ANSI_COLOR_RESET);
	}
}
char Scanner::getLetter()
{
	return file[index++];
}
