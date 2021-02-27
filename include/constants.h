/**@<constants.h>::**/

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

/* Definição de constantes */
#define MAXIDLEN 32
#define MAXSTBSIZE 0x10000

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
