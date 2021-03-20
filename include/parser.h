/**@<parser.h>::**/

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
  *   *17/03/2021: Adição de linecounter, columncounter e getnumtype
  * 
  */

#pragma once
/* "Prevenção" contra includes e declarações repetidas" */

/* Includes do sistema*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

/* Includes do projeto */
#include <tokens.h>
#include <constants.h>
#include <keywords.h>
#include <pseudocode.h>
#include <symtab.h>

/* Definição do cabeçalho de funções */
void declarative(void);
void imperative(void);
void vardecl(void);
void sbpdecl(void);
void varlist(void);
int typemod(void);
void sbphead(void);
void sbptail(void);
void formparm(void);
void stmt(void);
void ifstmt(void);
void whlstmt(void);
void rptstmt(void);
int expr(int);
int smpexpr(int);
int term(int);
int fact(int);
int getnumtype(const char *, int type);
char* tokenType(int expected);

void match(int expected);
int gettoken(FILE *);

/* Definição de variáveis externas */
extern int lookahead;
extern char lexeme[];
extern FILE *source;
extern int linecounter;
extern int columncounter;
