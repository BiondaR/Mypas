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
        printf("\tmovb %s, %s\n", src, dest);
    	break;
    case INT32:
        printf("\tmovl %s, %s\n", src, dest);
    	break;
    case INT64:
        printf("\tmovq %s, %s\n", src, dest);
    	break;
    case FLT32:
        printf("\tmovf %s, %s\n", src, dest);
    	break;
    case FLT64:
        printf("\tmovdf %s, %s\n", src, dest);
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

void cmp(int type)
{
    switch(type) {
    case BOOL:
	printf("\tcmpb auxb\n");
    	break;
    case INT32:
	printf("\tcmpl auxl\n");
    	break;
    case INT64:
	printf("\tcmpq auxq\n");
    	break;
    case FLT32:
	printf("\tcmpf auxf\n");
    	break;
    case FLT64:
	printf("\tcmpdf auxdf\n");
    	break;
    default:
    	;
    }
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
