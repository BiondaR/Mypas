#include <string.h>
#include <keywords.h>

char *keyword[] = { 
	"begin",
	"not",
	"or",
	"and",
	"div",
	"mod",
	"program",
	"procedure",
	"function",
	"var",
	"integer",
	"real",
	"double",
	"boolean",
	"if",
	"then",
	"else",
	"while",
	"do",
	"repeat",
	"until",
	"end",
};

int iskeyword(const char *name)
{
	int i;
	for (i = BEGIN; i < END + 1; i++) {
		if (strcmp(keyword[i - BEGIN], name) == 0) return i;
	}
	return 0;
}
