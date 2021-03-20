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
#include <stdio.h>
#include <stdlib.h>

/* Include do projeto */
#include <tokens.h>

/* Definição do cabeçalho de funções */
int gettoken(FILE *);
void mypas(void);

/* Declaração de variáveis */
extern int lookahead;
