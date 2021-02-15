/**@<mypas.c>::**/

#include <mypas.h>

int lookahead;
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
