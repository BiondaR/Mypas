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
  * Nicholas Seiti Handa
  * Nikolas Gomes de Sá
  * 
  */
  
 /*
  * Data de criação: 21/02/2021
  *  Atualizações:
  * 	*27/02/2021: Adição das modificações realizadas em aula no dia 24/02/21;
  *	*27/02/2021: Adição de comentários e de um contador de colunas;
  *	*02/03/2021: Adição de comentários;
  *	*16/03/2021: Correção de bugs na forma como o columncounter contava colunas;
  *     *20/03/2021: Correção de bugs na forma como o columncounter contava colunas;
  */

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

/* The column counter is incremented for each character read from the tape, in the functions in the lexer.c,
 * and is reseted to 1 for each new line started
 */
int columncounter = 1;
/* The line counter is incremented when a '\n' character is found */
int linecounter = 1;

/* The function below ignores spaces and comments that may be present on the tape */
void skipunused(FILE *tape)
{
  	int head;
	
	_skip_spaces_head:
	/* This section is responsible for ignoring space characters */
 	while ( isspace( head = getc(tape) ) ) {
		if (head == '\n') {
			columncounter = 1;
			linecounter++;
		} else {
			columncounter++;
		}
	}
	/* This section is responsible for ignoring comments, whose structure is: {comment} */
	if ( head == '{' ) {
		while ( (head = getc(tape)) != '}' && head != EOF ){
			if (head == '\n') {
				columncounter = 1;
				linecounter++;
			} else {
				columncounter++;
			}
		}
		/* After the end of the comment, it will be checked if there are more spaces characters and/or new comments on the tape */
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

/* The 'lookahead' variable gets each word/ID/number/etc of the tape. The lexeme is compared to the expected term and, 
 * if it didn't match, there is a semantic error (token mismatch).
 */
int lookahead;

/* The 'lexeme' variable is used to read each character/word in the tape and those words will go through a lexical analysis. */
char lexeme[MAXIDLEN+1];

int isID(FILE *tape)
{
	int i = 0;
	/* We store the tape character in the lexeme vector and if this is a letter [A-Za-z], it will already be considered ID. */
	if ( isalpha ( lexeme[i] = getc (tape) ) ) {
		/* We increase i to access the next vector position. */
		i++;
		
		columncounter++;
		/* If the character read is not alphanumeric, it only returns ID. Otherwise, we will continue accepting all 
         * characters that will come, checking only if the size doesn't exceed the limit of MAXIDLEN.
         */
		while ( isalnum ( lexeme[i] = getc (tape) ) ) {
			if(i < MAXIDLEN) {
				columncounter++;
				i++;
			}
		}
		/* So we consume all the characters and just return the last character 
         * read that it doesn't fit this category. 
         */
		ungetc ( lexeme[i], tape );
		lexeme[i] = 0;
		/* We make a last test to verify if the ID read is a reserved word.
		 * If so, we return the reserved word */
		int token = iskeyword(lexeme);
		if (token){
            return token;
        }
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
        columncounter++;
        if(lexeme[i] == '0'){
            i++;
            lexeme[i] = 0;
        }
        else{
            i++;
            while(isdigit(lexeme[i] = getc(tape))){
                if(i < MAXIDLEN){
			        columncounter++;
			        i++;
		        }
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
	        columncounter++;
            i++;
            token = FLOAT;
            /* At this stage, we can already consider it as FLOAT and while the imminent character of the tape is a digit, 
             * we will continue accepting it, since i doesn't exceed the maximum size of the vector (MAXIDLEN)
             */
            while(isdigit(lexeme[i] = getc(tape))){
                if(i < MAXIDLEN){
			        columncounter++;
			        i++;
		        }
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
	    columncounter++;
        i++;

        if(isdigit(lexeme[i] = getc(tape))){
	        columncounter++;
            i++;
            token = FLOAT;
            /* At this stage, we can already consider it as FLOAT and while the imminent character of the tape is a digit, we will continue
             * accepting it, since i doesn't exceed the maximum size of the vector (MAXIDLEN)
             */
            while(isdigit(lexeme[i] = getc(tape))){
                if(i < MAXIDLEN){
			        columncounter++;
			        i++;
		        }
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
            columncounter--;
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
            if(i < MAXIDLEN){
		        columncounter++;
		        i++;
	        }
		
            int plusminus;
            /* We use an auxiliary variable (plusminus) in case it's necessary to give back the character to the tape */
            if((lexeme[i] = plusminus = getc(tape)) == '+' || plusminus == '-'){
                if(i < MAXIDLEN){
			        columncounter++;
			        i++;
		        }
            }
            /* In case we don't receive an oplus from the tape and give back the character */
            else{
                ungetc(plusminus, tape);
                plusminus = 0;
            }
            /* To be exponential part, we must receive at least one digit after the 'e' or 'E' character */
            if(isdigit(lexeme[i] = getc(tape))){
                if(i < MAXIDLEN){
			        columncounter++;
			        i++;
		        }
		    
                token = FLOAT;
                /* At this stage, we can already consider it as FLOAT and while the imminent character of the tape is a digit, we will continue
                 * accepting it, since i doesn't exceed the maximum size of the vector (MAXIDLEN)
                 */
                while(isdigit(lexeme[i] = getc(tape))){
                    if(i < MAXIDLEN){
			            columncounter++;
			            i++;
		            }
                }
                /* We give back the character that isn't a digit to the tape */
                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
            }
            /* We say that the sequence doesn't characterize an exponential part, so we will give the characters back to the tape */
            else{
                ungetc(lexeme[i], tape);        /* Character isn't digit */
                i--;
                if(plusminus){                  /* If we received an oplus, we give it back */
                    i--;
		            columncounter--;
                    ungetc(plusminus, tape);
                }
                i--;
		        columncounter--;
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
	    columncounter++;
        /* We increase i to store the imminent character of the tape at the next vector position 
         * and check that it is in the [0-7] range. 
         */
        if(isdigit(lexeme[i] = getc(tape)) && lexeme[i] <= '7'){
            i++;
	        columncounter++;	
            /* At this stage, while the imminent character of the tape is an octal digit, we will continue
             * accepting it, since i doesn't exceed the maximum vector size (MAXIDLEN)
             */
            while(isdigit(lexeme[i] = getc(tape)) && lexeme[i] <= '7'){
                if(i < MAXIDLEN){
			        columncounter++;
			        i++;
		        }
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
	        columncounter--;
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
	    columncounter++;
        i++;
        /* We increment i to store the imminent character of the tape at the next vector position 
         * and check if it is 'x' or 'X'. 
         */
        if(toupper(lexeme[i] = getc(tape)) == 'X'){
	        columncounter++;	
            i++;
            /* We increment i to store the imminent character of the tape at the next vector position 
             * and check if the character is a hexadecimal digit. 
             */
            if(isxdigit(lexeme[i] = getc(tape))){
		    columncounter++;    
                i++;
                /* At this stage, while the imminent character of the tape is a hexadecimal digit, we will continue
                 * accepting it, since i doesn't exceed the maximum size of the vector (MAXIDLEN)
                 */
                while(isxdigit(lexeme[i] = getc(tape))){
                    if(i < MAXIDLEN){
			            columncounter++;
			            i++;
		            }
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
		        columncounter-=2;    
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
	    columncounter--;    
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
	/* Initially, the character ':' is expected in an assignment */
	if ( (lexeme[0] = getc(tape)) == ':' ) {
		columncounter++;
		/* We check if the next character is a '=' */
		if ( (lexeme[1] = getc(tape)) == '=' ) {
			/* In this case, we have an assignment */
			columncounter++;
			lexeme[2] = 0;
			return ASGN;
		}
		/* Otherwise, we don't have an assignment and the character read is returned to the tape */
		columncounter--;
		ungetc(lexeme[1], tape);
	}
	/* In this case, the character read was not ':' and there is no assignment */
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

/* A relational operator is used to construct Boolean expressions
 * In Pascal, a relational operator can have one of the following formats:
 * = (EQUAL)
 * > (GREATER THAN)
 * > = (GREATER THAN OR EQUAL)
 * <(LESS THAN)
 * <= (LESS THAN OR EQUAL)
 * <> (NOT EQUAL)
 */
int isRELOP(FILE *tape)
{
	int i = 0;
	switch (lexeme[i] = getc(tape)) {
	/* Case EQUAL */
	case '=':
		columncounter++;
		i++;
		lexeme[i] = 0;
		return '=';
	/* In this case, the operator can be > or >= */	
	case '>':
		columncounter++;	
		i++;
		/* We check if the next character on the tape is a '=' */
		if ( (lexeme[i] = getc(tape)) == '=' ) {
			/* If so, GREATER THAN OR EQUAL is returned */
			columncounter++;
			i++;
			lexeme[i] = 0;
			return GEQ;
		}
		/* Otherwise, the character read is returned to the tape and GREATER THAN is returned */
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '>';
	/* In this case, the operator can be <, <= or <> */
	case '<':
		columncounter++;
		i++;
		/* We check if the next character on the tape is a '=' */
		if ( (lexeme[i] = getc(tape)) == '=' ) {
			/* If so, LESS THAN OR EQUAL is returned */
			columncounter++;
			i++;
			lexeme[i] = 0;
			return LEQ;
		/* We check, then, if the next character on the tape is a '>' */
		} else if ( lexeme[i] == '>') {
			/* If so, NOT EQUAL is returned */
			columncounter++;
			i++;
			lexeme[i] = 0;
			return NEQ;
		}
		/* Otherwise, the character read is returned to the tape and LESS THAN is returned */ 
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '<';
	}
	/* If none of the checks are true, the character read is returned to the tape and 0 is returned */
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
	
	if ( (token = isASGN (source)) ) return token;

	if ( (token = isRELOP (source)) ) return token;

	lexeme[1] = 0;
	
	token = lexeme[0] = getc (source);
	columncounter++;
    
	return token;
}
