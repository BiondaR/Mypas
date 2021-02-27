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
  * Data de criação: 21/02/2021
  *  Atualizações:
  * 	*27/02/2021: Adição das modificações realizadas em aula no dia 24/02/21;
  *	*27/02/2021: Adição de comentários e de um contador de colunas;
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

/* O contador de colunas é incrementado a cada caractere lido da fita, em cada uma das funções presentes no lexer.c,
 * é decrementado a cada caractere devolvido à fita e é resetado em 1 a cada nova linha iniciada
 */
int columncounter = 1;
/* O contador de linhas é incrementado a cada caractere '\n' encontrado */
int linecounter = 1;

void skipunused(FILE *tape)
{
  	int head;
	
	_skip_spaces_head:
 	while ( isspace( head = getc(tape) ) ) {
		if (head == '\n') {
			columncounter = 1;
			linecounter++;
		} else {
			columncounter++;
		}
	}
	if ( head == '{' ) {
		while ( (head = getc(tape)) != '}' && head != EOF ){
			if (head == '\n') {
				columncounter = 1;
				linecounter++;
			} else {
				columncounter++;
			}
		}
		if (head == '}') { goto _skip_spaces_head; }
	}
	columncounter--;
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

int lookahead;
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
		columncounter--;
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
	columncounter--;
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
	    columncounter++;
            i++;
            lexeme[i] = 0;
        }
        else{
	    columncounter++;
            i++;
            while(isdigit(lexeme[i] = getc(tape))){
                if(i < MAXIDLEN){
			columncounter++;
			i++;
		}
            }
            columncounter--;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
        }
        return UINT;
    }
    /* In this case, the tape character was not a digit. So we just give it back and return 0. */
    columncounter--;
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
	    columncounter--;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
        }
        /* We don't find '.' on the tape, so it's just UINT.
         * the next step is to look for an exponential expression in sequence, which would make it FLOAT.
         */
        else{
	    columncounter--;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
        }
    }
    /* In this case, the imminent character of the tape is a point, 
     * so we should expect at least one digit before classifying it as FLOAT 
     */
    else if((lexeme[i] = getc(tape)) == '.'){]
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
	    columncounter-;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
        }
        /* In this case, we don't identify the tape sequence as NUM, so we won't check for exponential part 
         * and will only return 
         */
        else{
            columncounter-=2;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            ungetc('.', tape);
        }
    }
    /* In this case, we don't identify the tape sequence as NUM, so we won't check for exponential part 
     * and will only return 
     */
    else{
	columncounter--;
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
		columncounter--;
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
		columncounter-;
                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
            }
            /* We say that the sequence doesn't characterize an exponential part, so we will give the characters back to the tape */
            else{
		columncounter--;
                ungetc(lexeme[i], tape);        /* Character isn't digit */
                i--;
                if(plusminus){                  /* If we receive an oplus, we give it back */
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
	    columncounter--;	
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
	    columncounter--;	
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return OCT;
        }
        else{
            /* The first character read from the tape didn't correspond to an octal digit, therefore, 
             * we will give back the characters read to the tape and return 0 (Not Octal)
             */
	    columncounter-=2;
            ungetc(lexeme[i], tape);    /* Non octal character */
            i--;
            ungetc('0', tape);          /* Decrement i and gives '0' back */
            lexeme[i] = 0;
            return 0;
        }
    }
    /* The tape character didn't match '0', so we give back the character read to the tape and return 0 (Not Octal) */
    columncounter--;	
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
		columncounter--;    
                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
                return HEX;
            }
            else{
                /* The first character read from the tape didn't correspond to a hexadecimal digit, therefore, 
                 * we will return the characters read to the tape and return 0 (Not Hexadecimal)
                 */
		columncounter-=3;    
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
	columncounter-=2;    
        ungetc(lexeme[i], tape);
        ungetc('0', tape);
        return 0;
    }
    /* The first character of the tape isn't '0' and therefore not Hexadecimal */
    columncounter--;	
    ungetc(lexeme[i], tape);
    return 0;
}

/* ASGN = ":=" */
int isASGN(FILE *tape)
{
	/* Inicialmente o caractere ':' é esperado em uma atribuição */
	if ( (lexeme[0] = getc(tape)) == ':' ) {
		columncounter++;
		/* É checado se o próximo caractere é um '=' */
		if ( (lexeme[1] = getc(tape)) == '=' ) {
			/* Caso seja, temos uma atribuição */
			columncounter++;
			lexeme[2] = 0;
			return ASGN;
		}
		/* Caso contrário, não temos uma atribuição e o caractere lido é devolvido à fita */
		columncounter--;
		ungetc(lexeme[1], tape);
	}
	/* Neste caso, o caractere lido não é ':' e, portanto, não há atribuição */
	columncounter--;
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

/* Um operador relacional é utilizado para construir expressões booleanas
 * Em Pascal, um operador relacional pode ter um dos seguintes formatos:
 * = (EQUAL)
 * > (GREATER THAN)
 * >= (GREATER THAN OR EQUAL)
 * < (LESS THAN)
 * <= (LESS THAN OR EQUAL)
 * <> (NOT EQUAL)
 */
int isRELOP(FILE *tape)
{
	int i = 0;
	switch (lexeme[i] = getc(tape)) {
	/* Caso EQUAL */
	case '=':
		columncounter++;
		i++;
		lexeme[i] = 0;
		return '=';
	/* Neste caso, o operador pode ser > ou >= */		
	case '>':
		columncounter++;	
		i++;
			
		/* É verificado se o próximo caractere da fita é um '=' */	
		if ( (lexeme[i] = getc(tape)) == '=' ) {
			/* Caso seja, é retornado GREATER THAN OR EQUAL */
			columncounter++;
			i++;
			lexeme[i] = 0;
			return GEQ;
		}
		/* Do contrário, o caractere lido é devolvido à fita e é retornado GREATER THAN */
		columncounter--;
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '>';
	/* Neste caso, o operador pode ser <, <= ou <> */
	case '<':
		columncounter++;
		i++;
		/* É verificado se o próximo caractere da fita é um '=' */	
		if ( (lexeme[i] = getc(tape)) == '=' ) {
			/* Caso seja, é retornado LESS THAN OR EQUAL */
			columncounter++;
			i++;
			lexeme[i] = 0;
			return LEQ;
		/* Caso a primeira verificação seja negativa, é verificado se o próximo caractere da fita é um '>' */	
		} else if ( lexeme[i] == '>') {
			/* Caso seja, é retornado NOT EQUAL */
			columncounter++;
			i++;
			lexeme[i] = 0;
			return NEQ;
		}
		/* Do contrário, o caractere lido é devolvido à fita e é retornado LESS THAN */
		columncounter--;
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '<';
	}
	/* Caso nenhuma das verificações seja verdadeira, o caractere lido é devolvido à fita e é retornado 0 */
	columncounter--;
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
