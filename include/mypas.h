/**@<mypas.h>::**/

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

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>

/* Definição do cabeçalho de funções */
int gettoken(FILE *);
void mypas(void);
