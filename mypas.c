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
 * Nicholas Seiti Handa
 * Nikolas Gomes de Sá
 * 
 */
  
 /*
  * Data de criação: 21/02/2021
  * Atualizações:
  *	*02/03/2021: Adição de comentários;
  *
  */

#include <mypas.h>

/* Source code file pointer */
FILE *source;
/* Main function */
int main (int argc, char const *argv[])
{
	/* If there is an argument for the main function, we try to open the .pas file */
	if (argc > 1) {
		/* Attempt to open the file */
		source = fopen(argv[1], "r");
		/* Couldn't open the file */
		if(source == NULL) {
			fprintf(stderr, "%s: cannot open\n",
				argv[1]
			);
			exit (-1);
		}
	/* In this case, there is no argument, and the input will be read from the keyboard */	
	} else {
		source = stdin;
	}

	/* Gets the first token from the input source */
	lookahead = gettoken (source);

	/** initial symbol is mypas **/
	mypas();

	exit (0);
}
