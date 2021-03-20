/**@<tokens.h>::**/

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
  *
  */

#pragma once
/* "Prevenção" contra includes e declarações repetidas" */

/* Definição de constantes de tokens */
enum {
	ID = 1024,
	UINT,
	FLOAT,
	OCT,
	HEX,
	/* ASGN = ":=" */
	ASGN,
	/* GEQ = ">= */
	GEQ,
	/* LEQ = "<= */
	LEQ,
	/* NEQ = "<>= */
	NEQ,
};
