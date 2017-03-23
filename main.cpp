#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "scanner.h"
#include "parser.h"
#include "checker.h"
#include "color.h"
#include "token.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main (int argc, char ** argv)
{
	bool DEBUG = false;
	string path;
	if (argc > 1)
	{
		DEBUG = (argv[argc-1][0] == 'd' || argv[argc-1][0] == 'D' || argv[argc-1][0] == 'v' || argv[argc-1][0] == 'V');
		path = argv[1];
	}else{
		printf(ANSI_COLOR_RED "ERROR! wrong usage:\n" ANSI_COLOR_RESET);
		cout << argv[0] << " <PATH> <OPTIONAL DEBUG> <VV or V for verbose>" << endl;
		return -1;
	}
	if(DEBUG)
		printf(ANSI_COLOR_RED "Warning " ANSI_COLOR_YELLOW "Debug mode\n" ANSI_COLOR_RESET);
	Scanner *myScanner = new Scanner(path,DEBUG);
	for (std::vector<struct Token>::iterator it = myScanner->Tokens.begin() ; it != myScanner->Tokens.end(); ++it)
	{
		if(DEBUG)
		{
			printf(ANSI_COLOR_BLUE);
			cout << it->str << " ";
			printf(ANSI_COLOR_RESET);
		}
	}
	Parser * myParser = new Parser(myScanner->Tokens,DEBUG);
	struct TreeNode * AST = myParser->Parse();
	cout << "cleaning out..." << endl;
	delete (myParser);
	delete (myScanner);
	cout << "Done!" << endl;
	return 0;
}
