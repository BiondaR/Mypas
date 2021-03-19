/**@<lexer.h>::**/

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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Includes do projeto */
#include <tokens.h>
#include <constants.h>
#include <keywords.h>

/* Declaração de variáveis */
extern int columncounter;
extern int linecounter;
