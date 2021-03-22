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
  *     *20/03/2021: Correção de erros na criação de labels de função, correção de erros na chamada de return, adição de um move em expr;
  *     *20/03/2021: Correção de erro no iscompat;
  *     *20/03/2021: Adição de comentários no iscompat, sbpdecl e formparm;
  *     *21/03/2021: Adição de comentários no imperative, rtrn, stmt, ifstmt, whlstmt, rptstmt, isrelop e fact;
  *     *21/03/2021: Correção nos valores da passagem de parâmetro da função undeclared;
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

        /* Case acc_type = BOOL and syn_type has one of compatibility in the iscompat table return BOOL */
        case BOOL:
            if (syn_type == AND || syn_type == OR || syn_type == NOT || syn_type == BOOL) return BOOL;
            break;

        /* Case acc_type = INT32 and syn_type has one of compatibility in the iscompat table return syn_type 
        when we disconsidered '+', '-', '*', '/' */
        case INT32:
            if (syn_type >= INT32 && syn_type <= FLT64) return syn_type;
            break;

        /* Case acc_type = FLT32 and syn_type has one of compatibility in the iscompat table return syn_type 
        when we disconsidered '+', '-', '*', '/'. Or return FLT32 when syn_type = INT32 */
        case FLT32:
            if (syn_type >= FLT32 && syn_type <= FLT64) return syn_type;
            if (syn_type == INT32) return FLT32;
            break;

        /* Case acc_type = FLT64 and syn_type has one of compatibility in the iscompat table return FLT64 
        when we disconsidered '+', '-', '*', '/' */
        case FLT64:
            if (syn_type >= INT32 && syn_type <= FLT64) return FLT64;
    }
    /* In that ocasion we are checking INT32, INT64, FLT32 and FLT64 with '+', '-', '*', '/' */
    if (acc_type >= INT32 && acc_type <= FLT64) {
        /* if in these cases don't return acc_type, the types are incompatible*/
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
    /**/begin();/**/
    match(PROGRAM);
    match(ID);
    match(';'); 
    /*  lexical_level = 1 => fuctions, procedure and global variables */
    /**/lexical_level++;/**/
    declarative();
    imperative(); 
    /**/lexical_level--;/**/
    match ('.');
    /**/endcode(semantic_error);/**/
}

/*****************************************************************************
 * declarative -> vardecl sbpdecl
 *****************************************************************************/
void declarative(void) 
{ 
    vardecl();
    sbpdecl(); 
}

/*****************************************************************************
 * vardecl ->  [ VAR varlist : typemod ; { varlist : typemod ; } ]
 *****************************************************************************/
/*****************************************************************************
 * Variables declaration
 ****************************************************************************/
void vardecl(void)
{
    if (lookahead == VAR ) {
        match (VAR);
        /**/int first_pos;/**/
        /* Local variable declaration, because objtype = 1 => variable;
        transp_type = 1 => local storage;*/
        /**/objtype = transp_type = 1;/**/
        /* Preamble start */
        /**/preambledecl(lexical_level);/**/
        _varlist_head:
        /**/first_pos = symtab_next_entry;/**/
        varlist();
        match(':');
        /* Declare var type: integer, real, double or boolean as INT32, FLT32, FLT64 or BOOL */
        /**/int type = /**/ typemod();
        /* Add var type in symtab as Data_size: 
        INT32 or FLT32 => Data_size = 4, FLT64 => Data_size = 8, BOOL => Data_size = 2 */
        /**/symtab_update_type(first_pos, type);/**/
        /* Variable declaration in pseudocode */
        /**/preamble(type, first_pos, symtab_next_entry);/**/
        match(';');
        /* If lookahead is another var name (ID is var name in this case) goto _varlist_head to 
        repeat the previus steps */
        if (lookahead == ID) { 
            goto _varlist_head; 
        }
        else{
            /* End of preamble */
            /**/preambleend();/**/
        }
    } else {
        ;
    }
}

/*****************************************************************************
 * varlist -> ID { , ID }
 *****************************************************************************/
/*****************************************************************************
 * This function insert variables in the symtab
 ****************************************************************************/
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
/*****************************************************************************
 * Function/Procedure declaration
 ****************************************************************************/
void sbpdecl(void)
{
    /**/int isfunc = 0;/**/
    /**/char sbpname[MAXIDLEN+1];/**/
    /**/int symtab_sentinel;/**/
    _switch_head:
    switch (lookahead) {
        case FUNCTION:
            isfunc = 1;
        case PROCEDURE:
            /* Case lookahead is function
            objtype = 3 => function;
            Case lookahead isn't function
            objtype = 2 => procedure;  */
            /**/objtype = 2 + isfunc;/**/
            match(lookahead);
            /* Get function/procedure name */
            /**/strcpy(sbpname, lexeme);/**/
            /* Get position of function/procedure in the symtab */
            /**/int sbppos = symtab_append(sbpname, lexical_level, objtype, transp_type);/**/
            match(ID);
            /**/symtab_sentinel = symtab_next_entry;/**/
            /* When add function/procedure in symtab increment lexical_level
            to create a local variables, expression or statment */
            /**/lexical_level++;/**/
            /* See function/procedure parameter */
            formparm();
            if (isfunc) {
                isfunc = 0;
                match(':'); 
                /* Get the function type to store it in the symtab */
                /**/int rettype = /**/typemod();
                /* Store the function type in function position in the symtab */
                /**/symtab[sbppos].type = rettype;/**/
            } else {
                /* Store VOID in procedure position in the symtab, 
                because procedure doesn't return a value */
                /**/symtab[sbppos].type = VOID;/**/
            }
            match(';');
            /**/mkfplabel(sbpname);/**/
            /* Local declarations */
            declarative();
            /* Local that actually initialize the procedure/function */
            imperative();
            /* Exit function/procedure process */
            /**/lexical_level--;/**/
            /**/symtab_next_entry = symtab_sentinel;/**/
            match(';');
            /* End of function/procedure */
            /* If there is following function/procedure, do all procedures of function/procedure again */
            if (lookahead == FUNCTION || lookahead == PROCEDURE) { goto _switch_head; }
            /* Case didn't find a function/procedure do nothing */
        default:
            ;
    }
}

/*****************************************************************************
 * formparm -> [ ( [VAR] varlist : typemod { ; [VAR] varlist : typemod } ) ]
 *****************************************************************************/
/*****************************************************************************
 * Parameters declaration
 ****************************************************************************/
void formparm(void)
{
    /* If lookahead = '(', have parameter */
    if (lookahead == '(') {
        match('(');
        /**/int first_pos;/**/
        /**/objtype = 1;/**/
        parm_list:
        /**/first_pos = symtab_next_entry;/**/
        if (lookahead == VAR) { 
            /* Transp_type = 3 => passage by reference */
            match(VAR);
            /**/transp_type = 3;/**/
        } else {
            /* Transp_type = 2 => passage by value;  */
            /**/transp_type = 2;/**/
        }
        varlist();
        match(':');
        /**/int type = /**/typemod();
        /**/symtab_update_type(first_pos, type);/**/
        /* if there is another parameter go to indetificate it */
        if (lookahead == ';') { match(';'); goto parm_list; }
        match(')');
    }
    /* Otherwise don't have parameter */ 
    else {
        ;
    }
}

/*****************************************************************************
 * imperative -> BEGIN stmt { ; stmt } END
 *****************************************************************************/
/*****************************************************************************
 * Local that actually initialize the code
 ****************************************************************************/
void imperative(void)
{
    match(BEGIN);
    stmt_list:
    /* Enter the stmt */
    stmt();
    /* If there is another stmt go to the next one */
    if (lookahead == ';') { match(';'); goto stmt_list; }
    match(END);
}

/*****************************************************************************
 * RETURN declaration
 ****************************************************************************/
void rtrn(void)
{
    match(RETURN);
    /* Get the type to put it on a register thats correspond the type of the expression to return its type */
    /**/int exp_type = /**/expr(VOID);
    /* Print the return register type in pseudocode */
    /**/ret(exp_type); /**/
}

/******************************************************************************
 * stmt -> imperative | ifstmt | whlstmt | rptstmt | fact | <empty>
 *****************************************************************************/
/******************************************************************************
 * The function below decide the code "body" 
 * ***************************************************************************/
void stmt(void)
{
    /**/int fact_type;/**/
    /* In this switch we check the labels and go to yours respectives cases */
    switch (lookahead) {
        case BEGIN:
            imperative(); break;
        case IF:
            ifstmt(); break;
        case WHILE:
            whlstmt(); break;
        case REPEAT:
            rptstmt(); break;
        case RETURN:
            rtrn(); break;
        /* Case is '(', ID, BOOL, UINT, FLOAT, is a fact */
        case '(':
        case ID:
        case BOOL:
        case UINT:
        case FLOAT:
            /**/fact_type = /**/fact(VOID); break;
        /* Otherwise is <empty> */
        default:
            ;
    }
}

/*****************************************************************************
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 ****************************************************************************/
/*****************************************************************************
 * This function below create labels of ELSE and the end of ifstmt. 
 * And add them to the pseudocode
 ****************************************************************************/
/**/int loop_count = 1;/**/
void ifstmt(void)
{
    /* loop_count in this ifstmt is a counter to check how many times call IF and ELSE labels */
    /**/int expr_type, else_count, endif_count;/**/
    match(IF); 
    /**/expr_type = /**/expr(BOOL); 
    match(THEN);
    /* When expr_type = expr(BOOL); is false, go to ELSE label in the pseudocode */
    /**/gofalse(else_count = endif_count = loop_count++);/**/
    stmt();
    if (lookahead == ELSE) {
        match(ELSE);
        /**/
        /* When expr_type = expr(BOOL); is true and the IF stmt is complete, goto endif label in the pseudocode */
        golabel(endif_count = loop_count++); 
        /* Beginning of ELSE label */
        mklabel(else_count);
        /**/
        stmt();
    }
    /* Creation of endif label in the pseudocode */
    /**/mklabel(endif_count);/**/
}

/*****************************************************************************
 * whlstmt -> WHILE expr DO stmt
 *****************************************************************************/
/*****************************************************************************
 * This function is responsible for create a WHILE label
 * and the creation of a label that is the end of whlstmt.
 * And add them to the pseudocode
 ****************************************************************************/
void whlstmt(void)
{
    /* loop_count in this whlstmt is a counter to check how many times call WHILE label */
    /**/int expr_type, whlhead, whltail;/**/
    match(WHILE);
    /**/mklabel(whlhead = loop_count++);/**/
    /**/expr_type = /**/expr(BOOL);
    match(DO);
    /* When expr_type = expr(BOOL); is false, jump to the end whlstmt in the pseudocode */
    /**/gofalse(whltail = loop_count++);/**/
    stmt();
    /* After stmt is executed go back to while label and check again expr_type 
    to see if repeat the process or jump to the end of whlstmt in the pseudocode */
    /**/golabel(whlhead);/**/
    /**/mklabel(whltail);/**/
}

/*****************************************************************************
 * rptstmt -> REPEAT stmt { ; stmt } UNTIL expr
 *****************************************************************************/
/*****************************************************************************
 * This function is responsible for create a UNTIL label.
 * And add it to the pseudocode
 *****************************************************************************/
void rptstmt(void)
{
    /* loop_count in this whlstmt is a counter to check how many times call REPEAT label */
    /**/int expr_type; int replbl;/**/
    /**/mklabel(replbl = loop_count++);/**/
    stmt_head:
    stmt();
    if (lookahead == ';') { match(';'); goto stmt_head; }
    match(UNTIL);
    /**/expr_type = /**/expr(BOOL);
    /* When expr_type = expr(BOOL); is false, jump to REPEAT label in the pseudocode */
    /**/gofalse(replbl);/**/
}

/****************************************************************
 * The isrelop, checks the comparison signal cases.
 * Case lookahead is none of them, return 0, that means
 * is not a relop.
 ***************************************************************/
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


/* expr -> smpexpr [ relop smpexpr ] */
int expr(int expr_type)
{
    /**/int relop; int left_type, right_type;/**/
    /**/left_type = /**/smpexpr(VOID);
    if (isrelop()) {
        move(left_type, "acc", "aux");
        /**/relop = lookahead;/**/
        /**/if (expr_type != BOOL) expr_type = INCOMPTBL;/**/
        
        match(lookahead);
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
    /***/int term_type = /***/ term(smpexpr_type);/**/smpexpr_type = iscompat(smpexpr_type, term_type);/**/
     
    /**/if (signal == '-' || signal == NOT) negate(smpexpr_type);/**/
    /***/smpexpr_type = iscompat(smpexpr_type, term_type);/***/
    
    while ( lookahead == '+' || lookahead == '-' || lookahead == OR ) {
        /**/int oplus = lookahead;/**/
        /***/smpexpr_type = iscompat(smpexpr_type, oplus);/***/
         
        /**/push(smpexpr_type);/**/
        match (lookahead); 
        /***/term_type = /***/ term(smpexpr_type);
        
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
/**/int columns;/**/
/* term -> fact { (*) fact } */
int term(int term_type)
{ 
    /***/int fact_type = /***/fact(term_type); /**/term_type = iscompat(term_type, fact_type);/**/
     
    while ( lookahead == '*' || lookahead == '/' || lookahead == DIV | lookahead == MOD | lookahead == AND ) {
        /**/int otimes = lookahead;/**/
        /***/term_type = iscompat(term_type, otimes);/***/
        /**/push(term_type);/**/
       
        match (lookahead); 
        columns = columncounter;
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
    /**/int lin, col;/**/
	
    switch (lookahead) {
	/* In this case, is imminent an (expr) */
        case '(':
            match('('); /***/expr_type = /***/expr(fact_type); match(')');
	    /* Checks if initial fact type is compatible with obtained expr type */
            /***/fact_type = iscompat(fact_type, expr_type);/***/
            break;
	/* In the next cases, the fact is a boolean, integer or float type, respectively.
	 * We verify the type compatibility with "iscompat" and then, we generate a pseudocode
	 * to move the fact to an acc register. For integers and floats, we verify if the number
	 * needs 32 or 64 bits to be represented */
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
	/* In this case, the fact is a variable, proccedure or function */
        default:
            /**/strcpy(name, lexeme);/**/  
	    /* Saves the line and column where the lexeme was read. It may be used in the error messages */
	    /**/
	    lin = linecounter;
            col = columncounter - strlen(name); 
            /**/
		    
            match(ID);
	    /* Checks if the fact is an assignment */
            if (lookahead == ASGN) {
                /**** L-Value ****/
                match(ASGN); 
                /***/expr_type = /***/expr(fact_type);
                /**/
		/* If the variable wasn't declared, shows an error message and increments "semantic_error" */
                if ( symtab_lookup(name) < 0 ) {
                    /**/
		    undeclared(lin, col, name);
                    semantic_error++;
                    /**/
		} else {
		    /* If the object isn't a variable, shows an error message and increments "semantic_error" */
                    if (symtab[symtab_entry].objtype != 1) {
			/**/
                        fprintf(stderr, "Ln %d, Col %d: %s cannot be an L-Value\n", lin, col, name);
                        semantic_error++;
			/**/
                    } else {
			/* Checks the compatibilty between expr_type and the variable type */
                        fact_type = iscompat(expr_type, symtab[symtab_entry].type);
                        
                        /*** variable entry in symbol table is set in symtab_entry ***/
                        /**/move(fact_type, "acc", symtab[symtab_entry].offset);/**/
                    }
                }
                /**/
            } else {
                /**** R-Value ****/
                /**/
		/* If the variable wasn't declared, shows an error message and increments "semantic_error" */
                if ( symtab_lookup(name) < 0 ) {
		    /**/
                    undeclared(lin, col, name);
                    semantic_error++;
		    /**/
                } else {
                    /* Checks if the object is a variable, procedure or function */
                    switch(symtab[symtab_entry].objtype) {
                        case 1:
			    /* Checks the compatibilty between fact_type and the variable type */
                            fact_type = iscompat(fact_type, symtab[symtab_entry].type);
                            /*** variable entry in symbol table is set in symtab_entry ***/
                            move(fact_type, symtab[symtab_entry].offset, "acc");
                            break;
                        case 3:
			    /* Generate a pseudocode to call the function */
                            /**/callfp(symtab[symtab_entry].symbol);/**/
			    /* Matches the parameters */
                            if (lookahead == '(') {
                                match('(');
                                _expr_list:
                                expr(VOID);
                                if (lookahead == ',') { match(','); goto _expr_list; }
                                match(')');
                            }
			    /* Checks the compatibilty between fact_type and the variable type */
                            fact_type = iscompat(fact_type, symtab[symtab_entry].type);
                            /*** variable entry in symbol table is set in symtab_entry ***/
                            /**/move(fact_type, symtab[symtab_entry].offset, "acc");/**/
                            break;
                    }
                }
                /**/
            }
    }
    /* Returns the fact type */
    return fact_type;
}

/* This function is responsible for determining if a number type (integer or float) needs 
 * 32 or 64 bits for its representation */
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
		/**/
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
		/**/
	}
	
	/* Analysis of next buffer token */
	lookahead = gettoken(source);
}
