/**@<lexer.c>::**/

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
  * Data de criação: 19/01/2021
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <constants.h>
#include <keywords.h>
#include <lexer.h>

/* 
 * The lexical analyser is responsible for reading the flow of characters
 * that make up the program source and groups them into significant sequences, called lexemas. 
 * For each lexema, the lexical analyser produces as output a token in the 
 * format <name_token, value_atribute> that he moves on to the next step, 
 * the syntactic analysis. In a token, the component name_token is an abstract symbol that 
 * is used during the syntactic analysis, while the value_atribute refers to an entry in the 
 * symbol table for this token.
 * 
 */

int linecounter = 1;
void skipunused(FILE *tape)
{
  	int head;
	
	_skip_spaces_head:
 	while ( isspace( head = getc(tape) ) ) {
		if (head == '\n') {
			linecounter++;
		}
	}
	if ( head == '{' ) {
		while ( (head = getc(tape)) != '}' && head != EOF ){
		}
		if (head == '}') { goto _skip_spaces_head; }
	}
  	ungetc ( head, tape );
 
}
/* Now we need a predicate function to recognize a string
 * begining with a letter (alpha) followed by zero or more
 * digits and letters.
 *
 * REGEX: [A-Za-z][A-Za-z0-9]*
 *
 * isalpha(x) returns 1 if x \in [A-Za-z]
 *            returns 0 otherwise
 * isalnum(x) returns 1 if x \in [A-Za-z0-9]
 *            returns 0 otherwise
 */
char lexeme[MAXIDLEN+1];
int isID(FILE *tape)
{
	int i = 0;

	if ( isalpha ( lexeme[i] = getc (tape) ) ) {
		i++;
		while ( isalnum ( lexeme[i] = getc (tape) ) ) {
			i++;
		}
		ungetc ( lexeme[i], tape );
		lexeme[i] = 0;
		int token = iskeyword(lexeme); if (token) return token;
		return ID;
	}

	ungetc ( lexeme[i], tape );

	return 0;
}
/* Next, we have to implement a method to recognize decimal
 * pattern (unsigned int)
 * 
 * REGEX: [1-9][0-9]* | 0
 */
int isUINT(FILE *tape)
{
	int i = 0;

	if ( isdigit ( lexeme[i] = getc (tape) ) ) {
		if ( lexeme[i] == '0' ) {
			i++;
			lexeme[i] = 0;
			;
		} else {
			i++;
			while ( isdigit ( lexeme[i] = getc (tape) ) ) {
				i++;
			}
			ungetc ( lexeme[i], tape );
			lexeme[i] = 0;
		}
		return UINT;
	}

	ungetc ( lexeme[i], tape );

	return 0;
}
/** Trabalho de 08/12/2020: desenvolver um analisador léxico para ponto flutuante
 *
 * REGEX:
 * ( uint '.' [0-9]*  |  '.' [0-9]+ ) ee?  |  uint ee
 * uint = [1-9][0-9]* | 0
 * ee = [eE] ['+''-']? [0-9]+
 */
#include <string.h>
int isNUM(FILE *tape)
{
	int token = 0, i = 0;
	if ( (token = isUINT(tape)) ) {
		i += strlen(lexeme);
		if ( (lexeme[i] = getc(tape)) == '.' ) {
			i++;
			token = FLOAT;
			while ( isdigit(lexeme[i] = getc(tape)) ) {
				i++;
			}
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
		} else {
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
		}
	} else if ( (lexeme[i] = getc(tape)) == '.' ) {
		i++;
		/** decimal point has been already read **/
		if ( isdigit(lexeme[i] = getc(tape)) ) {
			i++;
			token = FLOAT;
			while ( isdigit(lexeme[i] = getc(tape)) ) {
				i++;
			}
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
		} else {
			/** after decimal point another char, non-digit has been read **/
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
			ungetc ('.', tape);
		}
	} else {
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
	}
	int eE;
	if ( token > 0 ) {
		if ( toupper(lexeme[i] = eE = getc(tape)) == 'E' ) {
			i++;
			int plusminus;
			if ( (lexeme[i] = plusminus = getc(tape)) == '+' || plusminus == '-' ) {
				i++;
			} else {
				ungetc (plusminus, tape);
				plusminus = 0;
			}
			if ( isdigit(lexeme[i] = getc(tape)) ) {
				i++;
				token = FLOAT;
				while ( isdigit(lexeme[i] = getc(tape)) ) {
					i++;
				}
				ungetc (lexeme[i], tape);
				lexeme[i] = 0;
			} else {
				ungetc (lexeme[i], tape);
				i--;
				if (plusminus) {
					i--;
					ungetc(plusminus, tape);
				}
				i--;
				ungetc(eE, tape);
			}
		} else {
			ungetc(eE, tape);
		}
	}
	lexeme[i] = 0;
	return token;
}
/* Octal pattern is defined as
 * REGEX: 0[0-7]+
 */
int isOCT(FILE *tape)
{
	int head;

	if ( ( head = getc (tape) ) == '0' ) {

		if ( isdigit( head = getc (tape) ) && head <= 7 ) {

			while  ( isdigit( head = getc (tape) ) && head <= 7 );

			ungetc( head, tape );

			return OCT;

		} else {

			ungetc( head, tape );

			ungetc( '0', tape );

			return 0;
		}
	}

	ungetc( head, tape );

	return 0;
}
/* Hexadecimalpattern is defined as
 * REGEX: 0[xX][0-9A-Fa-f]+
 */
int isHEX(FILE *tape)
{
	int head;
	int x;

	if ( ( head = getc (tape) ) == '0' ) {
		
		if ( ( x = getc (tape) ) == 'X' || x == 'x' ) {

			if ( isxdigit ( head = getc (tape) ) ) {

				while ( isxdigit ( head = getc (tape) ) );

				ungetc ( head, tape);

				return HEX;

			} else {

				ungetc ( head, tape );

				ungetc ( x, tape );

				ungetc ( '0', tape );

				return 0;

			}

		}

		ungetc ( x, tape );

		ungetc ( '0', tape );

		return 0;
	}

	ungetc (head, tape);

	return 0;
}

/* ASGN = ":=" */
int isASGN(FILE *tape)
{
	if ( (lexeme[0] = getc(tape)) == ':' ) {
		if ( (lexeme[1] = getc(tape)) == '=' ) {
			lexeme[2] = 0;
			return ASGN;
		}
		ungetc(lexeme[1], tape);
	}
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

int isRELOP(FILE *tape)
{
	int i = 0;
	switch (lexeme[i] = getc(tape)) {
	case '=':
		i++;
		lexeme[i] = 0;
		return '=';
	case '>':
		i++;
		if ( (lexeme[i] = getc(tape)) == '=' ) {
			i++;
			lexeme[i] = 0;
			return GEQ;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '>';
	case '<':
		i++;
		if ( (lexeme[i] = getc(tape)) == '=' ) {
			i++;
			lexeme[i] = 0;
			return LEQ;
		} else if ( lexeme[i] == '>') {
			i++;
			lexeme[i] = 0;
			return NEQ;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '<';
	}
	ungetc(lexeme[i], tape);
	return lexeme[i] = 0;
}

int gettoken(FILE *source)
{
	int token;

	skipunused (source);

	if ( (token = isID (source)) ) return token;

	if ( (token = isOCT (source)) ) return token;

	if ( (token = isHEX (source)) ) return token;

	if ( (token = isNUM (source)) ) return token;

	if ( (token = isRELOP (source)) ) return token;

	token = getc (source);

	return token;
}
