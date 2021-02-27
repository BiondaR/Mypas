/**@<symtab.c>::**/

/*
 * Bacharelado em Ciências da Computação
 * UNESP Rio Claro
 * Período Integral
 * 
 */

/* 
 * Desenvolvido pelo grupo 3:
 * Bionda Rozin
 * Matheus Missio Francisco
 * Nicholas Seiti Handa
 * Nikolas Gomes de Sá
 * 
 */
  
/*
 * Data de criação: 21/02/2021
 * Atualizações:
 * 	*27/02/2021: Adição das modificações realizadas em aula no dia 24/02/21
 *
 */

#include <symtab.h>

/*********************************
 * symtab.h::
typedef struct __symtab__ {
    char symbol[MAXIDLEN+1];
    char offset[MAXIDLEN+1];
    int type;
    int data_size;
    int lexical_level;
    int objtype;
    int transp_type;
}
SYMTAB;
*********************************/

SYMTAB symtab[MAXSTBSIZE];
int symtab_next_entry = 0;
int symtab_entry;
int symtab_lookup(const char *symbol)
{
    for (symtab_entry = symtab_next_entry - 1; symtab_entry > -1; symtab_entry--) {
        if (strcmp(symtab[symtab_entry].symbol, symbol) == 0) return symtab_entry;
    }
    return symtab_entry = -1;
}

int symtab_append(const char *symbol, int lexical_level, int objtype, int transp_type) {
    if ( symtab_lookup(symbol) < 0 || symtab[symtab_entry].lexical_level <= lexical_level) {
        strcpy(symtab[symtab_next_entry].symbol, symbol);
        sprintf(symtab[symtab_next_entry].offset, "%s[%d]", symbol, lexical_level); //é so um debug, pode ser melhorado
        symtab[symtab_next_entry].lexical_level = lexical_level;
        symtab[symtab_next_entry].objtype = objtype;
        symtab[symtab_next_entry].transp_type = transp_type;
        return symtab_next_entry++;
    } else {
	    /* essa parte pode ser modularizada (aula  17/2 min 11) */
        fprintf(stderr, "symtab_append: %s multiply defined in current lexical level %d\n", symbol, lexical_level);
        semantic_error++;
        return -2;
    }
}

void symtab_update_type(int start, int type) {
    int i;
    for (i = start; i < symtab_next_entry; i++) {
        symtab[i].type = type;
        switch(type) {
            case BOOL:
                symtab[i].data_size = 1; break;
            case INT32:
            case FLT32:
                symtab[i].data_size = 4; break;
            case INT64:
            case FLT64:
                symtab[i].data_size = 8; break;
            default:
                symtab[i].data_size = 0;
        }
    }
}
