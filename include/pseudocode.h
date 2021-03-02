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
#include <stdio.h>

/* Includes do projeto */
#include <constants.h>

/* Definição do cabeçalho de funções */
void not(void);
void negate(int);
void move(int type, const char *src, const char *dest);
void push(int type);
void add(int type);
void sub(int type);
void mul(int type);
void divl(int type);
void gofalse(int);
void mklabel(int loopnumber);
void golabel(int loopnumber);
