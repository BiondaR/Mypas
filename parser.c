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
  * 	*27/02/2021: Adição das modificações realizadas em aula no dia 24/02/21
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

/***
enum {
	INCOMPTBL = -1,
	VOID,
	BOOL,
	INT32,
	INT64,
	FLT32,
	FLT64,
};
***/

int iscompat(int acc_type, int syn_type)
{
    switch (acc_type) {
        case VOID:
            return syn_type;
        case BOOL:
            if (syn_type == AND || syn_type == OR || syn_type == NOT || syn_type == BOOL) return BOOL;
            break;
        case INT32:
            if (syn_type >= INT32 && syn_type <= FLT64) return syn_type;
            break;
        case FLT32:
            if (syn_type > FLT32 && syn_type <= FLT64) return syn_type;
            if (syn_type == INT32) return FLT32;
            break;
        case FLT64:
            if (syn_type >= INT32 && syn_type <= FLT64) return FLT64;
    }
    if (acc_type >= INT32 && acc_type <= FLT64) {
        switch(syn_type) {
            case'+':case'-':case'*':case'/':case DIV:case MOD:
                return acc_type;
        }
    }
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
    match(PROGRAM);
    match(ID);
    match(';'); 
    /**/lexical_level++;/**/
    declarative();
    imperative(); 
    /**/lexical_level--;/**/
    match ('.');
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
void vardecl(void)
{
    if (lookahead == VAR ) {
        match (VAR);
        /**/int first_pos;/**/
        /**/objtype = transp_type = 1;/**/
        _varlist_head:
        /**/first_pos = symtab_next_entry;/**/
        varlist();
        match(':');
        /**/int type = /**/ typemod();
        /**/symtab_update_type(first_pos, type);/**/
        match(';');
        if (lookahead == ID) { goto _varlist_head; }
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
    /**/symtab_append(lexeme, lexical_level, objtype, transp_type);/**/
    match(ID);
    if (lookahead == ',') { match(','); goto _match_id_head; }
}

/*****************************************************************************
 * typemod -> BOOLEAN | INTEGER | REAL | DOUBLE
 *****************************************************************************/
int typemod(void)
{
    /**/int type;/**/
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
            /**/objtype = 2 + isfunc;/**/
            match(lookahead);
            /**/strcpy(sbpname, lexeme);/**/
            /**/int sbppos = symtab_append(sbpname, lexical_level, objtype, transp_type);/**/
            match(ID);
            /**/symtab_sentinel = symtab_next_entry;/**/
            /**/lexical_level++;/**/
            formparm();
            if (isfunc) {
                isfunc = 0;
                match(':'); 
                /**/int rettype = /**/typemod();
                /**/symtab[sbppos].type = rettype;/**/
            } else {
                /**/symtab[sbppos].type = VOID;/**/
            }
            match(';');
            declarative();
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
        varlist();
        match(':');
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
    stmt();
    if (lookahead == ';') { match(';'); goto stmt_list; }
    match(END);
}

void rtrn(void)
{
    match(RETURN); 
    expr(VOID);
    match(';');
}

/*****************************************************************************
 * stmt -> imperative | ifstmt | whlstmt | rptstmt | fact | <empty>
 *****************************************************************************/
void stmt(void)
{
    /**/int fact_type;/**/
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
        case '(':
        case ID:
        case BOOL:
        case UINT:
        case FLOAT:
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
    /**/expr_type = /**/expr(BOOL);
    match(DO);
    /**/gofalse(whltail = loop_count++);/**/
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
    /**/printf(".L%d:\n", replbl = loop_count++);/**/
    stmt_head:
    stmt();
    if (lookahead == ';') { match(';'); goto stmt_head; }
    match(UNTIL);
    /**/expr_type = /**/expr(BOOL);
    /**/printf("\tgofalse .L%d\n", replbl);/**/
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
    /**/left_type = /**/smpexpr(VOID);
    
    if (isrelop()) {
        /**/relop = lookahead;/**/
        /**/if (expr_type != BOOL) expr_type = INCOMPTBL;/**/
        
        match(lookahead);
        
        /**/right_type = /**/smpexpr(left_type);
        /**/left_type = iscompat(left_type, right_type);/**/
        /**/if (left_type > 0) return expr_type;/**/
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
    /***/int fact_type = /***/fact(term_type); /**/term_type = iscompat(term_type, fact_type);/**/
    
    while ( lookahead == '*' || lookahead == '/' || lookahead == DIV | lookahead == MOD | lookahead == AND ) {
        /**/int otimes = lookahead;/**/
        /***/term_type = iscompat(term_type, otimes);/***/
        /**/push(term_type);/**/
        
        match (lookahead); 
        
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
int getnumtype(const char *); //IMPLEMENTAR

int fact(int fact_type) 
{
    /**/char name[MAXIDLEN+1];/**/ 
    /***/int expr_type;/***/
    
    switch (lookahead) {
        case '(':
            match('('); /***/expr_type = /***/expr(fact_type); match(')');
            /***/fact_type = iscompat(fact_type, expr_type);/***/
            break;
        case BOOL:
            /***/fact_type = iscompat(fact_type, BOOL);/***/
            /**/move(fact_type, lexeme, "acc");/**/
            
            match(BOOL);
            break;
        case UINT:
            /***/fact_type = iscompat(fact_type, INT32);/***/
            /**/move(fact_type, lexeme, "acc");/**/
            
            match(UINT);
            break;
        case FLOAT:
            /***/fact_type = iscompat(fact_type, FLT32);/***/
            /**/move(fact_type, lexeme, "acc");/**/
            
            match(FLOAT);
            break;
        default:
            /**/strcpy(name, lexeme);/**/
            
            match(ID);
            if (lookahead == ASGN) {
                /**** L-Value ****/
                match(ASGN); /***/expr_type = /***/expr(fact_type);
                /**/
                if ( symtab_lookup(name) < 0 ) {
                    fprintf(stderr, "%s undeclared\n", name);
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
                    fprintf(stderr, "%s undeclared\n", name);
                    semantic_error++;
                } else {
                    switch(symtab[symtab_entry].objtype) {
                        case 1:
                            fact_type = iscompat(fact_type, symtab[symtab_entry].type);
                            /*** variable entry in symbol table is set in symtab_entry ***/
                            move(fact_type, symtab[symtab_entry].offset, "acc");
                            break;
                        case 3:
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
	default:
		token_expected = "CHAR";
		break;
	}
	return token_expected;
}  

/* The match function is responsible for comparing the lookahead with the token expected by an expression/term/factor */
void match(int expected)
{	
	if (lookahead != expected) {
		char *token_expected;
		char *token_lookahead;
	
		token_expected = tokenType(expected);
		token_lookahead = tokenType(lookahead);
		if(token_lookahead == "CHAR") {
			fprintf(stderr,"token mismatch: expected %s whereas found %c\n",
		token_expected, lookahead);
		} else {
			fprintf(stderr,"token mismatch: expected %s whereas found %s\n",
		token_expected, token_lookahead);
		}
	}
	
	/* Analysis of next buffer token */
	lookahead = gettoken(source);
}
