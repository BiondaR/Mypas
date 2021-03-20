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
 * Nicholas Seiti Handa
 * Nikolas Gomes de Sá
 * 
 */

/*
 * Data de criação: 21/02/2021
 * Atualizações:
 * 	*27/02/2021: Adição das modificações realizadas em aula no dia 24/02/21
 *	*02/03/2021: Adição de comentários;
 *  *14/03/2021: Adição da verificação de symtab overflow;
 *  *18/03/2021: Adição de comentários;
 *  *20/03/2021: Modificação da representação do símbolo no pseudocode, na função symtab_append;
 *
 */

#include <symtab.h>

/*****************************************************************************
 * The symbol table is intended to add and consult symbols and information. 
 * It also aims to promote symbols types, when necessary, to execute fuctions.
 * Symbol table is required to check semantic of expressions
 ****************************************************************************/

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

/*****************************************************************************
 * Symbol query in the symbol table
 ****************************************************************************/
int symtab_lookup(const char *symbol)
{
    /* Go through the symtable */
    for (symtab_entry = symtab_next_entry - 1; symtab_entry > -1; symtab_entry--)
    {
        /* checks if the symbol exists in the symtab */
        if (strcmp(symtab[symtab_entry].symbol, symbol) == 0)
        /* return the position of the symbol in the symtab */
            return symtab_entry;
    }
    /* if doesn't have the symbol on the symtab, or the symtab is empty, return symtab_entry = -1 */
    return symtab_entry = -1;
}

/*****************************************************************************
 * objtype = 1 => variable; = 2 => procedure; = 3 => function
 * transp_type = 1 => local storage; = 2 => passage by value; = 3 => passage by reference
 * lexical_level => when enter in a function or a procedure increases the lexical level
 * Adding Symbol information in symbol table
 *****************************************************************************/
int symtab_append(const char *symbol, int lexical_level, int objtype, int transp_type)
{
    /* Check if there is some space in symtab to insert a new variable */
    if (symtab_next_entry < MAXSTBSIZE)
    {
        /* Check if the symbol doesn't exists in the symtab or have the same (or less) lexical level in the local aplication */
        if (symtab_lookup(symbol) < 0 || symtab[symtab_entry].lexical_level <= lexical_level)
        {
            /* Adds symbol name in the symtab */
            strcpy(symtab[symtab_next_entry].symbol, symbol);
            /* This sprintf shows the symbol and its position in the symtab in the pseudocode*/
            sprintf(symtab[symtab_next_entry].offset, " %s symtab[%d]", symbol, symtab_next_entry);
            /* Adds symbols lexical level in the symtab */
            symtab[symtab_next_entry].lexical_level = lexical_level;
            /* Adds symbols object type in the symtab */
            symtab[symtab_next_entry].objtype = objtype;
            /* Adds symbols transp_type in the symtab */
            symtab[symtab_next_entry].transp_type = transp_type;
            /* Return the next position to insert a new symbol*/
            return symtab_next_entry++;
        }
        else
        {
            /* There already have the symbol name in that lexical level */
            fprintf(stderr, "symtab_append: %s multiply defined in current lexical level %d\n", symbol, lexical_level);
            semantic_error++;
            return -2;
        }
    }

    /* There is no space in the symtab to add a new symbol */
    else
    {
        printf("Symtab overflow: no enough space in symtab to insert a new variable");
    }
}

/*****************************************************************************
 * Types promotion in symbol table
 ****************************************************************************/
void symtab_update_type(int start, int type)
{
    int i;

    for (i = start; i < symtab_next_entry; i++)
    {
        symtab[i].type = type;
        switch (type)
        {
        case BOOL:
        /* Case type is BOOL promote data_size of the symbol to 1 */
            symtab[i].data_size = 1;
            break;
        case INT32:
        case FLT32:
        /* Case type is INT32 or FLT32 promote data_size of the symbol to 4 */
            symtab[i].data_size = 4;
            break;
        case INT64:
        case FLT64:
        /* Case type is INT64 or FLT64 promote data_size of the symbol to 8 */
            symtab[i].data_size = 8;
            break;
        default:
        /* If type is none of this. Data_size of the symbol is 0 */
            symtab[i].data_size = 0;
        }
    }
}
