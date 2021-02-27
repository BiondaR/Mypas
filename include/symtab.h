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
  * 
  */

#pragma once
/* "Prevenção" contra includes e declarações repetidas" */

/* Includes do sistema */
#include <string.h>
#include <stdio.h>

/* Includes do projeto */
#include <constants.h>

/* Definição da tabela de símbolos, que possui o símbolo 
 * e seu tipo correspondente.
 *
 */
typedef struct __symtab__ {
    char symbol[MAXIDLEN+1];
    char offset[MAXIDLEN+1];
    int type;
    int data_size;
    /** inserted on feb 17th: **/
    int lexical_level;
    int objtype;
    int transp_type;
}
SYMTAB;

/* Definição do cabeçalho de funções. */
// int symtab_rtrvtype(const char *symbol, int lexical_level);
int symtab_lookup ( const char*);
int symtab_append(const char *symbol, int lexical_level, int objtype, int transp_type);
void symtab_update_type(int, int);

/* Definição de variáveis externas. */
extern SYMTAB symtab[MAXSTBSIZE];
extern int symtab_next_entry;
extern int semantic_error;
extern int symtab_entry;
