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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	/* We store the tape character in the lexeme vector and if this is a letter [A-Za-z], it will already be considered ID. */
	if ( isalpha ( lexeme[i] = getc (tape) ) ) {
		/* We increase i to access the next vector position. */
		i++;
		/* If the character read is not alphanumeric, it only returns ID. Otherwise, we will continue accepting all 
         	* characters that will come, checking only if the size doesn't exceed the limit of MAXIDLEN.
         	*/
		while ( isalnum ( lexeme[i] = getc (tape) ) ) {
			if(i < MAXIDLEN) i++;
		}
		/* So we consume all the characters and just return the last character 
         	* read that it doesn't fit this category. 
         	*/
		ungetc ( lexeme[i], tape );
		lexeme[i] = 0;
		/* We make a last test to verify if the ID read is a reserved word.
		 * If so, we return the reserved word */
		int token = iskeyword(lexeme);
		if (token) return token;
		/* If not, we return ID */
		return ID;
	}
	/* In this case, the character of the tape was not a letter. Therefore, we just give it back and return 0. */
	ungetc ( lexeme[i], tape );

	return 0;
}
/* Next, we have to implement a method to recognize decimal
 * pattern (unsigned int)
 * 
 * REGEX: [1-9][0-9]* | 0
 */
int isUINT(FILE* tape){
    int i = 0;
    /* We store the tape character in the lexeme vector and if this is a digit, it will already be considered UINT. */
    if(isdigit(lexeme[i] = getc(tape))){
        /* If the digit read is 0, only returns UINT. Otherwise, we will continue accepting all 
         * digits that will come, checking only if the size doesn't exceed the limit of MAXIDLEN.
         */
        if(lexeme[i] == '0'){
            i++;
            lexeme[i] = 0;
        }
        else{
            i++;
            while(isdigit(lexeme[i] = getc(tape))){
                if(i < MAXIDLEN) i++;
            }
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
        }
        return UINT;
    }
    /* In this case, the tape character was not a digit. So we just give it back and return 0. */
    ungetc(lexeme[i], tape);
    return 0;
}

/* NUM is the generic decimal pattern, which means it
 * can be an unsigned integer or a floating-point.
 *
 * UINT = [1-9][0-9]*  |  0
 *
 * FLOAT = ( UINT '.' [0-9]* | '.' [0-9]+ ) eE? | UINT eE
 *
 * eE = [eE] ['+''-']? [0-9]+
 */
int isNUM(FILE* tape){
    int token = 0, i = 0;
    /* Every UINT can be considered as NUM, so this first test is done */
    if((token = isUINT(tape))){
        i += strlen(lexeme);
        /* If there is a '.' after the integer part, the other digits characters are consumed */
        if((lexeme[i] = getc(tape)) == '.'){
            i++;
            token = FLOAT;
            /* At this stage, we can already consider it as FLOAT and while the imminent character of the tape is a digit, 
             * we will continue accepting it, since i doesn't exceed the maximum size of the vector (MAXIDLEN)
             */
            while(isdigit(lexeme[i] = getc(tape))){
                if(i < MAXIDLEN) i++;
            }
            /* We return the non-digit character to the tape and the next step 
             * is to check if there is an exponential expression in sequence
             */
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
        }
        /* We don't find '.' on the tape, so it's just UINT.
         * the next step is to look for an exponential expression in sequence, which would make it FLOAT.
         */
        else{
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
        }
    }
    /* In this case, the imminent character of the tape is a point, 
     * so we should expect at least one digit before classifying it as FLOAT 
     */
    else if((lexeme[i] = getc(tape)) == '.'){
        i++;

        if(isdigit(lexeme[i] = getc(tape))){
            i++;
            token = FLOAT;
            /* At this stage, we can already consider it as FLOAT and while the imminent character of the tape is a digit, we will continue
             * accepting it, since i doesn't exceed the maximum size of the vector (MAXIDLEN)
             */
            while(isdigit(lexeme[i] = getc(tape))){
                if(i < MAXIDLEN) i++;
            }
            /* We give back the non-digit character to the tape and the next step 
             * is to check if there is an exponential expression in sequence
             */
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
        }
        /* In this case, we don't identify the tape sequence as NUM, so we won't check for exponential part 
         * and will only return 
         */
        else{
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            ungetc('.', tape);
        }
    }
    /* In this case, we don't identify the tape sequence as NUM, so we won't check for exponential part 
     * and will only return 
     */
    else{
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
    }

    int e;
    /* This function will try to recognize a exponential part.
     * Considering that we have already classified the token as UINT or FLOAT
     */
    if(token > 0){
        /* If the character 'e' or 'E' is identified */
        if(toupper(lexeme[i] = e = getc(tape)) == 'E'){
            if(i < MAXIDLEN) i++;
            int plusminus;
            /* We use an auxiliary variable (plusminus) in case it's necessary to give back the character to the tape */
            if((lexeme[i] = plusminus = getc(tape)) == '+' || plusminus == '-'){
                if(i < MAXIDLEN) i++;
            }
            /* In case we don't receive an oplus from the tape and give back the character */
            else{
                ungetc(plusminus, tape);
                plusminus = 0;
            }
            /* To be exponential part, we must receive at least one digit after the 'e' or 'E' character */
            if(isdigit(lexeme[i] = getc(tape))){
                if(i < MAXIDLEN) i++;
                token = FLOAT;
                /* At this stage, we can already consider it as FLOAT and while the imminent character of the tape is a digit, we will continue
                 * accepting it, since i doesn't exceed the maximum size of the vector (MAXIDLEN)
                 */
                while(isdigit(lexeme[i] = getc(tape))){
                    if(i < MAXIDLEN) i++;
                }
                /* We give back the character that isn't a digit to the tape */
                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
            }
            /* We say that the sequence doesn't characterize an exponential part, so we will give the characters back to the tape */
            else{
                ungetc(lexeme[i], tape);        /* Character isn't digit */
                i--;
                if(plusminus){                  /* If we receive an oplus, we give it back */
                    i--;
                    ungetc(plusminus, tape);
                }
                i--;
                ungetc(e, tape);                /* Decrement i and gives 'e' or 'E' back */
            }
        }
        /* Give back 'e' or 'E' */
        else{
            ungetc(e, tape);
        }
    }
    lexeme[i] = 0;
    return token;
}

/* Octal pattern is defined as
 * REGEX: 0[0-7]+
 */
int isOCT(FILE* tape){
    int i = 0;
    /* Initially, a '0' is expected to characterize the Octal pattern */
    if((lexeme[i] = getc(tape)) == '0'){
        i++;
        /* We increase i to store the imminent character of the tape at the next vector position 
         * and check that it is in the [0-7] range. 
         */
        if(isdigit(lexeme[i] = getc(tape)) && lexeme[i] <= '7'){
            i++;
            /* At this stage, while the imminent character of the tape is an octal digit, we will continue
             * accepting it, since i doesn't exceed the maximum vector size (MAXIDLEN)
             */
            while(isdigit(lexeme[i] = getc(tape)) && lexeme[i] <= '7'){
                if(i < MAXIDLEN) i++;
            }
            /* So we consume all the octal digits and just give back the last character 
             * read that it doesn't fit this category. Finally, we return the OCT classification. 
             */
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return OCT;
        }
        else{
            /* The first character read from the tape didn't correspond to an octal digit, therefore, 
             * we will give back the characters read to the tape and return 0 (Not Octal)
             */
            ungetc(lexeme[i], tape);    /* Non octal character */
            i--;
            ungetc('0', tape);          /* Decrement i and gives '0' back */
            lexeme[i] = 0;
            return 0;
        }
    }
    /* The tape character didn't match '0', so we give back the character read to the tape and return 0 (Not Octal) */
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

/* Hexadecimalpattern is defined as
 * REGEX: 0[xX][0-9A-Fa-f]+
 */
int isHEX(FILE* tape){
    int i = 0;
    /* Initially a '0' is expected to characterize the Hexadecimal pattern */
    if((lexeme[i] = getc(tape)) == '0'){
        i++;
        /* We increment i to store the imminent character of the tape at the next vector position 
         * and check if it is 'x' or 'X'. 
         */
        if(toupper(lexeme[i] = getc(tape)) == 'X'){
            i++;
            /* We increment i to store the imminent character of the tape at the next vector position 
             * and check if the character is a hexadecimal digit. 
             */
            if(isxdigit(lexeme[i] = getc(tape))){
                i++;
                /* At this stage, while the imminent character of the tape is a hexadecimal digit, we will continue
                 * accepting it, since i doesn't exceed the maximum size of the vector (MAXIDLEN)
                 */
                while(isxdigit(lexeme[i] = getc(tape))){
                    if(i < MAXIDLEN) i++;
                }
                /* Therefore, we consume all hexadecimal digits and only give back the last character 
                 * read that it doesn't fit this category. Finally, we return the HEX classification 
                 */
                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
                return HEX;
            }
            else{
                /* The first character read from the tape didn't correspond to a hexadecimal digit, therefore, 
                 * we will return the characters read to the tape and return 0 (Not Hexadecimal)
                 */
                ungetc(lexeme[i], tape);        /* Non-hexadecimal character */
                i--;
                ungetc(lexeme[i], tape);        /* Decrement i and gives back 'x' or 'X' */
                i--;
                ungetc(lexeme[i], tape);        /* Decrement i and gives '0' back */
                lexeme[i] = 0;
                return 0;
            }
        }
        /* The tape character didn't match 'x' or 'X', so we give back the characters read to the tape and return 0 (Not HexaDecimal) */
        ungetc(lexeme[i], tape);
        ungetc('0', tape);
        return 0;
    }
    /* The first character of the tape isn't '0' and therefore not Hexadecimal */
    ungetc(lexeme[i], tape);
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

/* In the function below, we check the corresponding typing of the token read from the source code */
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
