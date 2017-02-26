#ifndef TOKEN_H
#define TOKEN_H
#include <string>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
	( std::ostringstream() << std::dec << x ) ).str()

#define LETTER 1
#define DIGIT 2
#define SYMBOLPLUS 3
#define SYMBOLMINUS 4
#define SYMBOLSTAR 5
#define SYMBOLSLASH 6
#define SYMBOLCOMP 7
#define SYMBOLEQUAL 8
#define SYMBOLSEMI 9
#define SYMBOLCOMMA 10
#define SYMBOLPAR 11
#define SYMBOLBRA 12
#define SYMBOLCURL 13
#define SYMBOLCOMMENT 14
struct Token{
	int type;
	int value;
	int line;
	std::string str;
};

#endif
