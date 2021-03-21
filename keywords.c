/**@<keywords.c>::**/

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
 */

#include <keywords.h>

/* 
 * A linguagem Pascal possui palavras reservadas que denotam diferentes comandos
 * e não podem ser utilizadas como identificadores. Essas palavras devem ser 
 * reconhecidas pelo compilador.
 * 
 */

/*
 * Below, they are defined as reserved words in the simplified grammar,
 * of the Pascal language.
 * 
 */

char *keyword[] = { 
	"begin",
	"not",
	"or",
	"and",
	"div",
	"mod",
	"program",
	"procedure",
	"function",
	"return",
	"var",
	"integer",
	"real",
	"double",
	"boolean",
	"if",
	"then",
	"else",
	"while",
	"do",
	"repeat",
	"until",
	"end",
};


/*
 * The function keywords is responsable to compare if given word is one of the reserved words defined above. 
 * n the case of a match, this function returns the value corresponding to the word matched, 
 * defined in keywords.h. Otherwise, it returns the value 0.
 */

int iskeyword(const char *name)
{
	int i;
	for (i = BEGIN; i < END + 1; i++) {
		if (strcmp(keyword[i - BEGIN], name) == 0) return i;
	}
	return 0;
}