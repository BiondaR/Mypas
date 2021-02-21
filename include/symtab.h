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

/* Definição da tabela de símbolos, que possui o símbolo 
 * e seu tipo correspondente.
 *
 */
typedef struct __symtab__ {
	char symbol[MAXIDLEN+1];
	int type;
}
SYMTAB;

/* Definição do cabeçalho de funções. */
int symtab_rtrvtype(const char *);
int symtab_lookup(const char *);

/* Definição de variáveis externas. */
extern SYMTAB symtab[MAXSTBSIZE];
extern int symtab_next_entry;
