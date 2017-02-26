#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "scanner.h"
#include "parser.h"
#include "checker.h"
#include "color.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main (int argc, char ** argv)
{
	bool DEBUG = false;
	if (argc > 1)
		DEBUG = (argv[1][0] == 'd' || argv[1][0] == 'D');
	if(DEBUG)
		printf(ANSI_COLOR_RED "Warning " ANSI_COLOR_YELLOW "Debug mode\n" ANSI_COLOR_RESET);

	Scanner *myScanner = new Scanner("test.txt",DEBUG);

//	delete (myScanner);
	return 0;
}
