/**@<mypas.c>::**/

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

#include <mypas.h>

FILE *source;
int main (int argc, char const *argv[])
{
	if (argc > 1) {
		source = fopen(argv[1], "r");
		if(source == NULL) {
			fprintf(stderr, "%s: cannot open\n",
				argv[1]
			);
			exit (-1);
		}
	} else {
		source = stdin;
	}

	lookahead = gettoken (source);

	/** initial symbol is mypas **/
	mypas();

	exit (0);
}
