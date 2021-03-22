/**@<pseudocode.h>::**/

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
  *  Alterações:
  *   *17/03/2021: Adição de cmp;
  *   *19/03/2021: Adição da função ret, callfunc, mkfunclabel;
  *   *20/03/2021: Adição de begin, preambuledecl, preambuleend, undeclared e endcode;
  */
#pragma once
/* "Prevenção" contra includes e declarações repetidas" */

/* Includes do sistema */
#include <stdio.h>

/* Includes do projeto */
#include <tokens.h>
#include <constants.h>
#include <symtab.h>

/* Definição do cabeçalho de funções */
void negate(int type);
void move(int type, const char *src, const char *dest);
void push(int type);
void add(int type);
void sub(int type);
void mul(int type);
void divl(int type);
void cmp (int relop, int type, char *aux, char *acc);
void ret(int type);
void undeclared (int line, long int col, char *name);
void callfp(char *funcname);
void mkfplabel(char *funcname);
void gofalse(int);
void mklabel(int loopnumber);
void golabel(int loopnumber);
void begin(void);
void preambledecl(int lexlevel);
void preamble(int type, int symtab_entry, int sym_ntx_entry);
void preambleend(void);
void endcode(int semantic_errors);

extern SYMTAB symtab[MAXSTBSIZE];
extern int symtab_entry;
