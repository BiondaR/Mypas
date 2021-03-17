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
 * Matheus Missio Francisco
 * Nicholas Seiti Handa
 * Nikolas Gomes de Sá
 * 
 */
  
 /*
  * Data de criação: 21/02/2021
  *
  */

#include <pseudocode.h>

/* 
 * The pseudocode is a result of the efficient modularization of the parser, 
 * which aims to encapsulate the methods and organize the code, one of the possible applications 
 * of this program is to transform a 'printf' into a primitive, in other words, a semantic action being reduced.
*/

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

void cmp (int relop, int type, char *aux, char *acc) {
    char *suffix, *instr;
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
    printf("\t%s%s %s%s, %s%s\n", instr, suffix, aux, suffix, acc, suffix);
}

void gofalse(int loopnumber)
{
    printf("\tgofalse .L%d\n", loopnumber);
}

void mklabel(int loopnumber) {
    printf(".L%d:\n", loopnumber);
}

void golabel(int loopnumber)
{
    printf("\tgoto .L%d\n", loopnumber);
}
