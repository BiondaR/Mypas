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

/* Includes do sistema*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Includes do projeto */
#include <tokens.h>
#include <constants.h>
#include <keywords.h>
#include <pseudocode.h>
#include <symtab.h>

/* Definição de constantes utilizadas no parser.c */
enum {
	INCOMPTBL = -1,
	VOID,
	BOOL,
	INT32,
	INT64,
	FLT32,
	FLT64,
};

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
void match(int expected);
int gettoken(FILE *);

/* Definição de variáveis externas */
extern int lookahead;
extern char lexeme[];
extern FILE *source;
