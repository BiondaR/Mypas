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
 * Matheus Missio Francisco 
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
 * Abaixo, estão definidas as palavras reservadas da gramática simplificada da
 * linguagem Pascal.
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
 * A função iskeywords é responsável por comparar se uma palavra presente
 * no código fonte é uma das palavras reservadas definidas acima. No
 * caso positivo, a função retorna o valor correspondente à palavra escrita,
 * definido em keywords.h. Caso contrário, apenas retorna o valor 0.
 * 
 */

int iskeyword(const char *name)
{
	int i;
	for (i = BEGIN; i < END + 1; i++) {
		if (strcmp(keyword[i - BEGIN], name) == 0) return i;
	}
	return 0;
}
