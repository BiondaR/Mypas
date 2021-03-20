/**@<parser.c>::**/

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
  * Atualizações:
  * 	*27/02/2021: Adição das modificações realizadas em aula no dia 24/02/21;
  *	    *02/03/2021: Adição da função tokentype e modificação da função match;
  *	    *16/03/2021: melhoria na mensagem de erro da função match;
  *     *17/03/2021: Implementação da função getnumtype;
  *     *17/03/2021: Correção da chamada da função cmp;
  *     *19/03/2021: Adição de comentários no iscompat, mypas, vardecl, varlist e typemod;
  *     *20/03/2021: Adição de modularizações e comentários;
  *
  */

#include <parser.h>

/* 
 * The syntatic analysis (parsing) is used for a formal analysis by some sentence or string, 
 * according  to a formal grammar, resulting in a parse tree. The parsing step is important 
 * to facilitate the process of compiling/interpreting a source code
 * 
 */

/** iscompat table: **/
/***************************************************************************************************
 *        bool   int32   int64   flt32   flt64     '+'     '-'     '*'     '/'    NOT    OR    AND
 * bool   bool   -----   -----   -----   -----    -----   -----   -----   -----  bool   bool   bool
 * int32  ----   int32   int64   flt32   flt64    int32   int32   int32   int32  ----   ----   ----
 * int64  ----   int64   int64   flt32   flt64    int64   int64   int64   int64  ----   ----   ----
 * flt32  ----   flt32   flt32   flt32   flt64    flt32   flt32   flt32   flt32  ----   ----   ----
 * flt64  ----   flt64   flt64   flt64   flt64    flt64   flt64   flt64   flt64  ----   ----   ----
 ***************************************************************************************************/

/**************************************************************************************** 
 * This is a implementation of inherited attributes. This function check, 
 * using as a base the type table above, the compatibility 
 * between the left (acc_type) and right (syn_type) type 
****************************************************************************************/
int iscompat(int acc_type, int syn_type)
{
    /* In that switch every acc_type with syn_type without '+', '-', '*', '/' */
    switch (acc_type) {
        case VOID:
            return syn_type;

        /* if in this case doesn't return BOOL the types are incompatible*/
        case BOOL:
            if (syn_type == AND || syn_type == OR || syn_type == NOT || syn_type == BOOL) return BOOL;
            break;

        /* if in this case doesn't return syn_type the types are incompatible*/
        case INT32:
            if (syn_type >= INT32 && syn_type <= FLT64) return syn_type;
            break;

        /* if in this case doesn't return syn_type or return FLT32 the types are incompatible*/
        case FLT32:
            if (syn_type > FLT32 && syn_type <= FLT64) return syn_type;
            if (syn_type == INT32) return FLT32;
            break;

        /* if in this case doesn't return FLT64 the types are incompatible*/
        case FLT64:
            if (syn_type >= INT32 && syn_type <= FLT64) return FLT64;
    }
    /* In that ocasion we are checking INT32, INT64, FLT32 and FLT64 with '+', '-', '*', '/' */
    if (acc_type >= INT32 && acc_type <= FLT64) {
        /* if in these cases don't return acc_type the types are incompatible*/
        switch(syn_type) {
            case'+':case'-':case'*':case'/':case DIV:case MOD:
                return acc_type;
        }
    }
    /* return INCOMPTBL where the types encounter in the type table is dashed */
    return INCOMPTBL;
}

/*****************************************************************************
 * mypas -> PROGRAM ID ; declarative imperative .
 *****************************************************************************/
/**/int lexical_level = 0;/**/
/*** objtype = 1 => variable; = 2 => procedure; = 3 => function ***/
/**/int objtype;/**/
/*** transp_type = 1 => local storage; = 2 => passage by value; = 3 => passage by reference ***/
/**/int transp_type;/**/
void mypas(void) 
{ 
    begin();
    match(PROGRAM);
    match(ID);
    match(';'); 
    /* lexical_level of fuctions, procedure and global variables */
    /**/lexical_level++;/**/
    //printf("\n /**/ declarative call /**/");
    declarative();
    //printf("\n /**/ imperative call /**/");
    imperative(); 
    /**/lexical_level--;/**/
    match ('.');
    endcode(semantic_error);
}

/*****************************************************************************
 * declarative -> vardecl sbpdecl
 *****************************************************************************/
void declarative(void) 
{ 
    //printf("\n /**/ vardecl call /**/");
    vardecl();
    //printf("\n /**/ sbpdecl call /**/");
    sbpdecl(); 
}

/*****************************************************************************
 * vardecl ->  [ VAR varlist : typemod ; { varlist : typemod ; } ]
 *****************************************************************************/
void vardecl(void)
{
    if (lookahead == VAR ) {
        match (VAR);
        /**/int first_pos;/**/
        /* Local variable declaration, because objtype = 1 => variable;
        transp_type = 1 => local storage;*/
        /**/objtype = transp_type = 1;/**/
        /* Preambule start */
        preambuledecl(lexical_level);
        _varlist_head:
        /**/first_pos = symtab_next_entry;/**/
        //printf("\n /**/ varlist call /**/");
        varlist();
        match(':');
        //printf("\n /**/ typemod call /**/");
        /* Declare var type: integer, real, double or boolean as INT32, FLT32, FLT64 or BOOL */
        /**/int type = /**/ typemod();
        /* Add var type in symtab as Data_size: 
        INT32 or FLT32 => Data_size = 4, FLT64 => Data_size = 8, BOOL => Data_size = 2 */
        /**/symtab_update_type(first_pos, type);/**/
        /* Variable declaration in pseudocode */
        preambule(type, first_pos, symtab_next_entry);
        match(';');
        /* If lookahead is another var name (ID is var name in this case) goto _varlist_head to 
        repeat the previus steps */
        if (lookahead == ID) { 
            goto _varlist_head; 
        }
        else{
            /* End of preambule */
            preambuleend();
        }
    } else {
        ;
    }
}

/*****************************************************************************
 * varlist -> ID { , ID }
 *****************************************************************************/
/**/int semantic_error = 0;/**/
void varlist(void)
{
    _match_id_head:
    /* Analyzes and, if it's possible, add to the symbol table */
    /**/symtab_append(lexeme, lexical_level, objtype, transp_type);/**/
    match(ID);
    /* If more than one variable is defined goto _match_id_head and add to symtab */
    if (lookahead == ',') { match(','); goto _match_id_head; }
}

/*****************************************************************************
 * typemod -> BOOLEAN | INTEGER | REAL | DOUBLE
 *****************************************************************************/
int typemod(void)
{
    /**/int type;/**/
    /* Declare types of var or functions: integer, real, double or boolean as INT32, FLT32, FLT64 or BOOL */
    switch (lookahead) {
        case INTEGER:
            /**/type = INT32;/**/
            break;
        case REAL:
            /**/type = FLT32;/**/
            break;
        case DOUBLE:
            /**/type = FLT64;/**/
            break;
        default:
            /**/type = BOOL;/**/
    }
    match(lookahead);
    return type;
}

/*****************************************************************************
 * sbpdecl -> { PROCEDURE ID formparm ; declarative imperative ; | FUNCTION ID formparm : typemod ; declarative imperative ; }
 *****************************************************************************/
/**/int func_count = 1;/**/
/**/int proc_count = 1;/**/
void sbpdecl(void)
{
    /**/int isfunc = 0;/**/
    /**/int proc_counter;/**/
    /**/char sbpname[MAXIDLEN+1];/**/
    /**/int symtab_sentinel;/**/
    _switch_head:
    switch (lookahead) {
        case FUNCTION:
            isfunc = 1;
        case PROCEDURE:
            /**/objtype = 2 + isfunc;/**/
            match(lookahead);
            /**/strcpy(sbpname, lexeme);/**/
            /**/int sbppos = symtab_append(sbpname, lexical_level, objtype, transp_type);/**/
            match(ID);
            /* label procedure */
            //mkproclabel(1);
            /**/symtab_sentinel = symtab_next_entry;/**/
            /**/lexical_level++;/**/
            //printf("\n /**/ formparm call /**/");
            formparm();
            if (isfunc) {
                /*aqui entra o label de função*/
                isfunc = 0;
                match(':'); 
                //printf("\n /**/ typemod call /**/");
                mkfunclabel(1);
                /**/int rettype = /**/typemod();
                /**/symtab[sbppos].type = rettype;/**/
            } else {
                mkproclabel(proc_counter = proc_count++);
                /**/symtab[sbppos].type = VOID;/**/
            }
            match(';');
            //printf("\n /**/ declarative call /**/");
            declarative();
            //printf("\n /**/ imperative call /**/");
            imperative();
            /**/lexical_level--;/**/
            /**/symtab_next_entry = symtab_sentinel;/**/
            match(';');
            if (lookahead == FUNCTION || lookahead == PROCEDURE) { goto _switch_head; }
        default:
            ;
    }
}

/*****************************************************************************
 * formparm -> [ ( [VAR] varlist : typemod { ; [VAR] varlist : typemod } ) ]
 *****************************************************************************/
void formparm(void)
{
    if (lookahead == '(') {
        match('(');
        /**/int first_pos;/**/
        /**/objtype = 1;/**/
        parm_list:
        /**/first_pos = symtab_next_entry;/**/
        if (lookahead == VAR) { 
            match(VAR);
            /**/transp_type = 3;/**/
        } else {
            /**/transp_type = 2;/**/
        }
        //printf("\n /**/ varlist call /**/");
        varlist();
        match(':');
        //printf("\n /**/ typemod call /**/");
        /**/int type = /**/typemod();
        /**/symtab_update_type(first_pos, type);/**/
        if (lookahead == ';') { match(';'); goto parm_list; }
        match(')');
    } else {
        ;
    }
}

/*****************************************************************************
 * imperative -> BEGIN stmt { ; stmt } END
 *****************************************************************************/
void imperative(void)
{
    match(BEGIN);
    stmt_list:
    //printf("\n /**/ stmt call /**/");
    stmt();
    if (lookahead == ';') { match(';'); goto stmt_list; }
    match(END);
}

void rtrn(void)
{
    match(RETURN);
    /* como passar o tipo?*/
    ret(INT32); 
    expr(VOID);
}

/*****************************************************************************
 * stmt -> imperative | ifstmt | whlstmt | rptstmt | fact | <empty>
 *****************************************************************************/
void stmt(void)
{
    /**/int fact_type;/**/
    switch (lookahead) {
        case BEGIN:
            //printf("\n /**/ imperative call /**/");
            imperative(); break;
        case IF:
            //printf("\n /**/ ifstmt call /**/");
            ifstmt(); break;
        case WHILE:
            //printf("\n /**/ whlstmt call /**/");
            whlstmt(); break;
        case REPEAT:
            //printf("\n /**/ rptstmt call /**/");
            rptstmt(); break;
        case RETURN:
            //printf("\n /**/ rtrn call /**/");
            rtrn(); break;
        case '(':
        case ID:
        case BOOL:
        case UINT:
        case FLOAT:
            //printf("\n /**/ fact call /**/");
            /**/fact_type = /**/fact(VOID); break;
        default:
            ;
    }
}

/*****************************************************************************
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 *****************************************************************************/
/**/int loop_count = 1;/**/
void ifstmt(void)
{
    /**/int expr_type, else_count, endif_count;/**/
    match(IF); 
    //printf("\n /**/ expr call /**/");
    /**/expr_type = /**/expr(BOOL); 
    match(THEN);
    /**/gofalse(else_count = endif_count = loop_count++);/**/
    stmt();
    if (lookahead == ELSE) {
        match(ELSE);
        /**/
        golabel(endif_count = loop_count++); 
        mklabel(else_count);
        /**/
        //printf("\n /**/ stmt call /**/");
        stmt();
    }
    /**/mklabel(endif_count);/**/
}

/*****************************************************************************
 * whlstmt -> WHILE expr DO stmt
 *****************************************************************************/
void whlstmt(void)
{
    /**/int expr_type, whlhead, whltail;/**/
    match(WHILE);
    /**/mklabel(whlhead = loop_count++);/**/
    //printf("\n /**/ expr call /**/");
    /**/expr_type = /**/expr(BOOL);
    match(DO);
    /**/gofalse(whltail = loop_count++);/**/
    //printf("\n /**/ stmt call /**/");
    stmt();
    /**/golabel(whlhead);/**/
    /**/mklabel(whltail);/**/
}

/*****************************************************************************
 * rptstmt -> REPEAT stmt { ; stmt } UNTIL expr
 *****************************************************************************/
void rptstmt(void)
{
    /**/int expr_type; int replbl;/**/
    /**/mklabel(replbl = loop_count++);/**/
    stmt_head:
    //printf("\n /**/ stmt call /**/");
    stmt();
    if (lookahead == ';') { match(';'); goto stmt_head; }
    match(UNTIL);
    //printf("\n /**/ expr call /**/");
    /**/expr_type = /**/expr(BOOL);
    /**/gofalse(replbl);/**/
}

/* expr -> smpexpr [ relop smpexpr ] */
int isrelop(void)
{
    switch(lookahead) {
        case '=':
        case '>':
        case '<':
        case GEQ:
        case NEQ:
        case LEQ:
            return lookahead;
    }
    return 0;
}

int expr(int expr_type)
{
    /**/int relop; int left_type, right_type;/**/
    //printf("\n /**/ smpexpr call /**/");
    /**/left_type = /**/smpexpr(VOID);
    //printf("\n /**/ isrelop call /**/");
    if (isrelop()) {
        /**/relop = lookahead;/**/
        /**/if (expr_type != BOOL) expr_type = INCOMPTBL;/**/
        
        match(lookahead);
        //printf("\n /**/ smpexpr call /**/");
        /**/right_type = /**/smpexpr(left_type);
        /**/left_type = iscompat(left_type, right_type);/**/
        /**/if (left_type > 0) {
            cmp(relop, right_type, "aux", "acc");
            return expr_type;
            }/**/
    } else {
        /**/expr_type = iscompat(expr_type, left_type);/**/
    }
    /**/return expr_type;/**/
}

/* smpexpr -> ['+''-'] term { (+) term } */
int smpexpr(int smpexpr_type) 
{
    /**/int signal = 0;/**/
    if (lookahead == '+' || lookahead == '-' || lookahead == NOT) {
        /**/signal = lookahead;/**/
        /***/smpexpr_type = iscompat(smpexpr_type, signal);/***/
        
        match(lookahead);
    }
    //printf("\n /**/ term call /**/");
    /***/int term_type = /***/ term(smpexpr_type);/**/smpexpr_type = iscompat(smpexpr_type, term_type);/**/
    /**/if (signal == '-' || signal == NOT) negate(smpexpr_type);/**/
    /***/smpexpr_type = iscompat(smpexpr_type, term_type);/***/
    
    while ( lookahead == '+' || lookahead == '-' || lookahead == OR ) {
        /**/int oplus = lookahead;/**/
        /***/smpexpr_type = iscompat(smpexpr_type, oplus);/***/
        /**/push(smpexpr_type);/**/
        
        //printf("\n /**/ term call /**/");
        match (lookahead); /***/term_type = /***/ term(smpexpr_type);
        
        /***/smpexpr_type = iscompat(smpexpr_type, term_type);/***/
        /**/move(smpexpr_type, "acc", "aux");/**/
        
        /**/
        switch(oplus) {
            case '+':
                add(smpexpr_type);
                break;
            default:
                sub(smpexpr_type);
                
        }
        /**/
    }
    
    /***/return smpexpr_type;/***/
}

/* term -> fact { (*) fact } */
int term(int term_type)
{ 
    //printf("\n /**/ fact call /**/");
    /***/int fact_type = /***/fact(term_type); /**/term_type = iscompat(term_type, fact_type);/**/
    
    while ( lookahead == '*' || lookahead == '/' || lookahead == DIV | lookahead == MOD | lookahead == AND ) {
        /**/int otimes = lookahead;/**/
        /***/term_type = iscompat(term_type, otimes);/***/
        /**/push(term_type);/**/
        
        match (lookahead); 
        //printf("\n /**/ fact call /**/");
        /***/fact_type = /***/fact(term_type);
        
        /***/term_type = iscompat(term_type, fact_type);/***/
        
        /**/move(term_type, "acc", "aux");/**/
        
        /**/
        switch(otimes) {
            case '*':
                mul(term_type);
                break;
            default:
                divl(term_type);
                
        }
        /**/
    }
    
    /***/return term_type;/***/
}

/*  fact ->  ( expr )
 *       | n
 *       | v [ = expr ]
 */


int fact(int fact_type) 
{
    /**/char name[MAXIDLEN+1];/**/ 
    /***/int expr_type, numtype;/***/
    /**/int columns;/**/
    
    switch (lookahead) {
        case '(':
            //printf("\n /**/ expr call /**/");
            match('('); /***/expr_type = /***/expr(fact_type); match(')');
            /***/fact_type = iscompat(fact_type, expr_type);/***/
            break;
        case BOOL:
            /***/fact_type = iscompat(fact_type, BOOL);/***/
            /**/move(fact_type, lexeme, "acc");/**/
            
            match(BOOL);
            break;
        case UINT:
            numtype = getnumtype(lexeme, UINT);
            /***/fact_type = iscompat(fact_type, numtype);/***/
            /**/move(fact_type, lexeme, "acc");/**/
            
            match(UINT);
            break;
        case FLOAT:
            numtype = getnumtype(lexeme, FLOAT);
            /***/fact_type = iscompat(fact_type, numtype);/***/
            /**/move(fact_type, lexeme, "acc");/**/            
            match(FLOAT);
            break;
        default:
            /**/strcpy(name, lexeme);/**/
            
            columns = columncounter;
            
            match(ID);
            if (lookahead == ASGN) {
                /**** L-Value ****/
                //printf("\n /**/ expr call /**/");
                match(ASGN); /***/expr_type = /***/expr(fact_type);
                /**/
                if ( symtab_lookup(name) < 0 ) {
                    undeclared(linecounter, columns, name);
                    semantic_error++;
                } else {
                    if (symtab[symtab_entry].objtype != 1) {
                        fprintf(stderr, "%s cannot be an L-Value\n", name);
                        semantic_error++;
                    } else {
                        fact_type = iscompat(expr_type, symtab[symtab_entry].type);
                        /*** variable entry in symbol table is set in symtab_entry ***/
                        move(fact_type, "acc", symtab[symtab_entry].offset);                        
                    }
                }
                /**/
            } else {
                /**** R-Value ****/
                /**/
                if ( symtab_lookup(name) < 0 ) {
                    undeclared(linecounter, columns, name);
                    semantic_error++;
                } else {
                    /*** objtype = 1 => variable; = 2 => procedure; = 3 => function ***/
                    switch(symtab[symtab_entry].objtype) {
                        case 1:
                            fact_type = iscompat(fact_type, symtab[symtab_entry].type);
                            /*** variable entry in symbol table is set in symtab_entry ***/
                            move(fact_type, symtab[symtab_entry].offset, "acc");
                            break;
                        case 3:
                            callfunc(lexical_level); /*verrr*/
                            if (lookahead == '(') {
                                match('(');
                                _expr_list:
                                expr(VOID);
                                if (lookahead == ',') { match(','); goto _expr_list; }
                                match(')');
                            }
                            fact_type = iscompat(fact_type, symtab[symtab_entry].type);
                            /*** variable entry in symbol table is set in symtab_entry ***/
                            move(fact_type, symtab[symtab_entry].offset, "acc");
                            break;
                    }
                }
                /**/
            }
    }
    return fact_type;
}

int getnumtype(const char * lexeme, int type){
    
    switch(type){
        case UINT:
            if((strtold(lexeme,NULL)) <= INT_MAX && (strtold(lexeme,NULL)) >= INT_MIN){
                return INT32;
            }
            else{
                return INT64;
            }
            break;

        default:
            if((strtold(lexeme,NULL)) <= FLT_MAX && (strtold(lexeme,NULL)) >= FLT_MIN){
                return FLT32;
            }
            else{
                return FLT64;
            }
            break;
    }
}


/* This function is used to replace the token value by its correspondant string.*/
char* tokenType(int expected) {
	char* token_expected;

	switch(expected) {
	case ID:
		token_expected = "ID";
		break;
	case UINT:
		token_expected = "UINT";
		break;
	case FLOAT:
		token_expected = "FLOAT";
		break;
	case OCT:
		token_expected = "OCT";
		break;
	case HEX:
		token_expected = "HEX";
		break;
	case ('('):
		token_expected = "(";
		break;    
	case (')'):
		token_expected = ")";
		break;
	case (ASGN):
		token_expected = ":=";
		break;
	case (GEQ):
		token_expected = ">=";
		break;
	case ('='):
		token_expected = "=";
		break;
	case ('>'):
		token_expected = ">";
		break;
	case (LEQ):
		token_expected = "<=";
		break;
	case ('<'):
		token_expected = "<";
		break;
	case (NEQ):
		token_expected = "<>=";
		break;
	case (-1):
		token_expected = "EOF";
		break;
	case BEGIN:
		token_expected = "BEGIN";
		break;
	case NOT:
		token_expected = "NOT";
		break;
	case OR:
		token_expected = "OR";
		break;
	case AND:
		token_expected = "AND";
		break;
	case DIV:
		token_expected = "DIV";
		break;
	case MOD:
		token_expected = "MOD";
		break;    
	case PROGRAM:
		token_expected = "PROGRAM";
		break;
	case PROCEDURE:
		token_expected = "PROCEDURE";
		break;
	case FUNCTION:
		token_expected = "FUNCTION";
		break;
	case RETURN:
		token_expected = "RETURN";
		break;
    	case VAR:
		token_expected = "VAR";
		break;
    	case INTEGER:
		token_expected = "INTEGER";
		break;
    	case REAL:
		token_expected = "REAL";
		break;
    	case DOUBLE:
		token_expected = "DOUBLE";
		break;
    	case BOOLEAN:
		token_expected = "BOOLEAN";
		break;
    	case IF:
		token_expected = "IF";
		break;
    	case THEN:
		token_expected = "THEN";
		break;
    	case ELSE:
		token_expected = "ELSE";
		break;
    	case WHILE:
		token_expected = "WHILE";
		break;
    	case DO:
		token_expected = "DO";
		break;
   	case REPEAT:
		token_expected = "REPEAT";
		break;
   	case UNTIL:
		token_expected = "UNTIL";
		break;
	default:
		token_expected = "CHAR";
		break;
	}
	return token_expected;
}  

/* The match function is responsible for comparing the lookahead with the token expected by an expression/term/factor */
/* The error messages shows the expected token, the line and the column where the error occurred*/
void match(int expected)
{	
	if (lookahead != expected) {
		char *token_expected;
		char *token_lookahead;
	
		token_expected = tokenType(expected);
		token_lookahead = tokenType(lookahead);
		if(token_lookahead == "CHAR") {
			fprintf(stderr,"Ln %d, Col %d: token mismatch: expected %s whereas found %c\n",
		linecounter, (columncounter - 1), token_expected, lookahead);
		} else {
			fprintf(stderr,"Ln %d, Col %ld: token mismatch: expected %s whereas found %s\n",
		linecounter, (columncounter - strlen(lexeme)), token_expected, token_lookahead);
		}
	}
	
	/* Analysis of next buffer token */
	lookahead = gettoken(source);
}
