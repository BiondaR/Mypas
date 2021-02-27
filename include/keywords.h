/**@<keywords.h>::**/

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

#pragma once
/* "Prevenção" contra includes e declarações repetidas" */

/* Includes do sistema */
#include <string.h>

/* Definição do cabeçalho da função iskeywors */
int iskeyword(const char *);

/* Definição das constantes correspondentes às palavras chave */
enum {
	BEGIN =  10001,
	NOT,
	OR,
	AND,
	DIV,
	MOD,
	PROGRAM,
	PROCEDURE,
	FUNCTION,
	RETURN,
	VAR,
	INTEGER,
	REAL,
	DOUBLE,
	BOOLEAN,
	IF,
	THEN,
	ELSE,
	WHILE,
	DO,
	REPEAT,
	UNTIL,
	END,
};

