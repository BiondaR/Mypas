#include <string.h>
#include <stdio.h>
#include <constants.h>
#include <symtab.h>
/*********************************
 * symtab.h::
typedef
struct __symtab__ {
	char symbol[MAXIDLEN+1];
	int type;
}
SYMTAB;
*********************************/
SYMTAB symtab[MAXSTBSIZE];
int symtab_next_entry = 0;
int symtab_lookup(const char *symbol)
{
	int i;
	for (i = symtab_next_entry - 1; i > -1; i--) {
		if (strcmp(symtab[i].symbol, symbol) == 0) return i;
	}
	return i;
}
/**@symtab_rtrvtype::
 * returns the type assigned to the symbol passed as argument,
 * otherwise, if the symbol was not found, it returns a negative integer
 */
int symtab_rtrvtype(const char *symbol)
{
	int i = symtab_lookup(symbol);
	if (i < 0) {
		fprintf(stderr, "symtab_rtrvtype: %s undeclared\n", symbol);
		return i;
	}
	return symtab[i].type;
}
