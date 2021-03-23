/**@<pseudocode.c>::**/

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
  *  Alterações:
  *     *16/03/2021: Correção do cmp;
  *     *17/03/2021: Melhoeria na função move;
  *     *19/03/2021: Adição da função ret, callfunc, mkfunclabel;
  *     *20/03/2021: Adição de begin, preambuledecl, preambuleend, undeclared, begin e endcode;
  *     *21/03/2021: Adição de comentários no resto do pseudocódigo;
  *     *22/03/2021: Adição da função pop;
  *
  */

#include <pseudocode.h>

/* 
 * The pseudocode is a result of the efficient modularization of the parser, 
 * which aims to encapsulate the methods and organize the code, one of the possible applications 
 * of this program is to transform a 'printf' into a primitive, in other words, a semantic action being reduced.
*/

/** suffix table: **/
/****************************************************************
 *          BOOL    INT32    INT64    FLT32    FLT64
 * suffix     b       l        q        f        df
 * 
 ***************************************************************/

/****************************************************************
 * If there is no suffix, any code will be generated
 ***************************************************************/ 

/** instr table: **/
/****************************************************************
 *          '>'    '<'    '='    NEQ    LEQ    GEQ
 * instr     gt     lt     eq    neq    leq    geq
 * 
 ***************************************************************/

/****************************************************************
 * The function below negate the register depending on the type.
 * And write a correspondent pseudocode
 ***************************************************************/
void negate(int type)
{
    switch(type) {
    case BOOL:
    	printf("\tnotb accb\n");
    	break;
    case INT32:
    	printf("\tnegl accl\n");
    	break;
    case INT64:
    	printf("\tnegq accq\n");
    	break;
    case FLT32:
    	printf("\tnegf accf\n");
    	break;
    case FLT64:
    	printf("\tnegdf accdf\n");
    	break;
    default:
    	;
    }
}

/****************************************************************
 * The function below move the value of source to the destination.
 * With the direction of the data flow from left to right.
 * And write a pseudocode 
 * with the suffixes corresponding to the correct types
 ***************************************************************/
void move(int type, const char *src, const char *dest)
{
    switch(type) {
    case BOOL:
        printf("\tmovb %sb, %sb\n", src, dest);
    	break;
    case INT32:
        printf("\tmovl %sl, %sl\n", src, dest); 
    	break;
    case INT64:
        printf("\tmovq %sq, %sq\n", src, dest);
    	break;
    case FLT32:
        printf("\tmovf %sf, %sf\n", src, dest);
    	break;
    case FLT64:
        printf("\tmovdf %sdf, %sdf\n", src, dest);
    	break;
    default:
    	;
    }
}

/****************************************************************
 * Stores the value of the register at the top of the stack
 ***************************************************************/
void push(int type)
{
    switch(type) {
    case BOOL:
        printf("\tpushb accb\n");
    	break;
    case INT32:
        printf("\tpushl accl\n");
    	break;
    case INT64:
        printf("\tpushq accq\n");
    	break;
    case FLT32:
        printf("\tpushf accf\n");
    	break;
    case FLT64:
        printf("\tpushdf accdf\n");
    	break;
    default:
    	;
    }
}

/****************************************************************
 * The pop function takes the value from the top of the stack
 * and stores it in the register
 ***************************************************************/
void pop(int type)
{
    switch(type) {
    case BOOL:
        printf("\tpopb accb\n");
    	break;
    case INT32:
        printf("\tpopl accl\n");
    	break;
    case INT64:
        printf("\tpopq accq\n");
    	break;
    case FLT32:
        printf("\tpopf accf\n");
    	break;
    case FLT64:
        printf("\tpopdf accdf\n");
    	break;
    default:
    	;
    }
}

/****************************************************************
 * The function below adds the value of aux to the acc accumulator.
 * With the direction of the data flow from left to right as intel processor.
 * And write a pseudocode 
 * with the suffixes corresponding to the correct types
 ***************************************************************/
void add(int type)
{
    switch(type) {
    case INT32:
        printf("\taddl auxl, accl\n");
    	break;
    case INT64:
        printf("\taddq auxq, accq\n");
    	break;
    case FLT32:
        printf("\taddf auxf, accf\n");
    	break;
    case FLT64:
        printf("\tadddf auxdf, accdf\n");
    	break;
    default:
    	;
    }
}

/****************************************************************
 * The function below takes the aux value to subtract the acc accumulator.
 * With the direction of the data flow from left to right as intel processor.
 * And write a pseudocode 
 * with the suffixes corresponding to the correct types
 ***************************************************************/
void sub(int type)
{
    switch(type) {
    case INT32:
        printf("\tsubl auxl, accl\n");
    	break;
    case INT64:
        printf("\tsubq auxq, accq\n");
    	break;
    case FLT32:
        printf("\tsubf auxf, accf\n");
    	break;
    case FLT64:
        printf("\tsubdf auxdf, accdf\n");
    	break;
    default:
    	;
    }
}

/****************************************************************
 * The function below take the aux value to multiplicate the previus destination 
 * register in the function move and store the result in the same destination 
 * register.
 * With the direction of the data flow from left to right as intel processor.
 * And write a pseudocode 
 * with the suffixes corresponding to the correct types
 ***************************************************************/
void mul(int type)
{
    switch(type) {
    case INT32:
        printf("\tmul auxl\n");
    	break;
    case INT64:
        printf("\tmuq auxq\n");
    	break;
    case FLT32:
        printf("\tmuf auxf\n");
    	break;
    case FLT64:
        printf("\tmudf auxdf\n");
    	break;
    default:
    	;
    }
}

/****************************************************************
 * The function below take the aux value to divide the previus destination 
 * register in the function move, store the quotient in the same destination 
 * register and store the rest result in another register predetermined by 
 * local processor.
 * With the direction of the data flow from left to right as intel processor.
 * And write a pseudocode 
 * with the suffixes corresponding to the correct types
 ***************************************************************/
void divl(int type)
{
    switch(type) {
    case INT32:
	printf("\tdivl auxl\n");
    	break;
    case INT64:
	printf("\tdivq auxq\n");
    	break;
    case FLT32:
	printf("\tdivf auxf\n");
    	break;
    case FLT64:
	printf("\tdivdf auxdf\n");
    	break;
    default:
    	;
    }
}

/****************************************************************
 * The cmp function write a pseudocode that represents
 * assembly compare in local processor with its types
 ***************************************************************/
void cmp (int relop, int type, char *aux, char *acc) {
    char *suffix = "", *instr = "";
    /* Prepare suffix type */
    switch(type) {
        case BOOL:
            suffix = "b";
            break;
        case INT32:
            suffix = "l";
            break;
        case INT64:
            suffix = "q";
            break;
        case FLT32:
            suffix = "f";
            break;
        case FLT64:
            suffix = "df";
            break;
        default:
            ;
    }
    /* Prepare instruction */
    switch(relop) {
        case '>':
            instr = "gt";
            break;
        case '<':
            instr = "lt";
            break;
        case '=':
            instr = "eq";
            break;
        case NEQ:
            instr = "neq";
            break;
        case LEQ:
            instr = "leq";
            break;
        case GEQ:
            instr = "geq";
            break;
        default:
            ;     
    }  
    if ( suffix != "" && instr != "" ){
        printf("\t%s%s %s%s, %s%s\n", instr, suffix, aux, suffix, acc, suffix);
    }
    
}

/****************************************************************
 * Generate a pseudocode for "return" command.
 * Each type, determined by the returned element, has a code modifier (a suffix).
 * If there is an invalid type, the function doesn't generate any code
 ****************************************************************/
void ret(int type) {
    switch(type) {
    case INT32:
	    printf("\treturnl accl\n\n");
    	break;
    case INT64:
	    printf("\treturnq accq\n\n");
    	break;
    case FLT32:
	    printf("\treturnf accf\n\n");
    	break;
    case FLT64:
	    printf("\treturndf accdf\n\n");
    	break;
    default:
    	;
    }
}

/****************************************************************
 * Generate a pseudocode when a variable is undeclared and 
 * shows the line and column where this semantic error occours
 *****************************************************************/
void undeclared (int line, long int col, char *name) {
    printf("Ln %d, Col %ld: %s undeclared\n", line, col, name);
}

/**************************************************************** 
 * Call a function/procedure by its label
 ****************************************************************/
void callfp(char *funcname){
    printf("\tcall %s\n", funcname);
}

/****************************************************************
 * Create a label to a function/procedure
 ****************************************************************/
void mkfplabel(char *funcname){
    printf("%s:\n", funcname);
}

/****************************************************************
 * Generate a pseudocode instruction that indicates if a cmp 
 * instruction is false, it may go to the indicated label
 ****************************************************************/
void gofalse(int loopnumber)
{
    printf("\tgofalse .L%d\n", loopnumber);
}

/****************************************************************
 * Create a generic label
 ****************************************************************/
void mklabel(int loopnumber) {
    printf(".L%d:\n", loopnumber);
}

/****************************************************************
 * This instruction indicates that the code may jump 
 * to the indicated label
 ****************************************************************/
void golabel(int loopnumber)
{
    printf("\tgoto .L%d\n", loopnumber);
}

/****************************************************************
 * This is the header of the pseudocode, that indicates the 
 * code was compiled with our compilator, and shows the 
 * developers names
 ****************************************************************/
void begin(void){
    printf("/****************************************************************************\n");
    printf("\tThis is a simplified pascal compiler");
    printf("\n\tDeveloped by:");
    printf("\n\t\tBionda Rozin");
    printf("\n\t\tNicholas Seiti Handa");
    printf("\n\t\tNikolas Gomes de Sá");
    printf("\n\tIts use is restrict for academic proposes, only");
    printf("\n\t23/03/2021\n");
    printf("****************************************************************************/\n");
}

/****************************************************************
 * Generates a pseudocode that indicates the begin of the 
 * preamble, where the variables are declared
 ****************************************************************/
void preambledecl(int lexlevel) {
    printf("\n.decl:\n");
    printf("\tlex_level %d\n", lexlevel);
}

/****************************************************************
 * Generates the preamble itself, where each variable for that
 * lexical level is declared
 ****************************************************************/
void preamble(int type, int symtab_entry, int sym_ntx_entry){
    int flag = 1;
    /* Indicates the type of the variable */
    for(int i = symtab_entry; i < sym_ntx_entry; i++){
        switch(type){
            case BOOL:
    	        printf("\t.b");
    	        break;
        case INT32:
    	        printf("\t.l");
    	        break;
        case INT64:
    	        printf("\t.q");
    	        break;
        case FLT32:
    	        printf("\t.f");
    	        break;
        case FLT64:
    	        printf("\t.df");
    	        break;
        default:
    	       flag = 0;
    }

        if (flag == 1){
            printf("\t%s\n", symtab[i].symbol);
        }
	/* Doesn't print anything if there is an invalid type */
        else{
            flag = 1;
        }
    }
}

/****************************************************************
 * Generates the end of the preamble and indicates the begin 
 * of the code itself
 ****************************************************************/
void preambleend(void){
    printf(".end_decl\n\n");
    printf(".code:\n\n");
}

/****************************************************************
 * Indicates the end of the code and shows how many semantic 
 * errors occurred during the code
 ****************************************************************/
void endcode(int semantic_errors){
    printf("\n.end_code\n");
    printf("\tThere was %d semantic errors in this code\n\n", semantic_errors);
}
