typedef
struct __symtab__ {
	char symbol[MAXIDLEN+1];
	int type;
}
SYMTAB;
int symtab_rtrvtype(const char *);
int symtab_lookup(const char *);
extern SYMTAB symtab[MAXSTBSIZE];
extern int symtab_next_entry;
