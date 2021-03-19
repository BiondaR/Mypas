/**@<symtab.h>::**/

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
  *   *02/03/2021: Adição de comentários;
  * 
  */

#pragma once
/* Prevention of repeateds includes and declarations */

/* System Includes */
#include <string.h>
#include <stdio.h>

/* Project Includes */
#include <constants.h>

/* 
 * Definition of symbol table, that has the symbol and its corresponding informations
 * like your type, position on the symtab, lexical level, etc.
 */
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

/* Definition of functions headers. */
int symtab_lookup ( const char*);
int symtab_append(const char *symbol, int lexical_level, int objtype, int transp_type);
void symtab_update_type(int, int);

/* Definition of extern variables. */
extern SYMTAB symtab[MAXSTBSIZE];
extern int symtab_next_entry;
extern int semantic_error;
extern int symtab_entry;
