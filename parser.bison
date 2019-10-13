/*
Declare token types at the top of the bison file,
causing them to be automatically generated in parser.tab.h
for use by scanner.c.
*/

// start here 

%token	 TOKEN_EOF
%token	 TOKEN_T_ARRAY
%token	 TOKEN_T_AUTO
%token	 TOKEN_T_BOOLEAN
%token	 TOKEN_T_CHAR
%token	 TOKEN_ELSE
%token	 TOKEN_FALSE
%token	 TOKEN_FOR
%token	 TOKEN_FUNCTION
%token	 TOKEN_IF
%token	 TOKEN_T_INTEGER
%token	 TOKEN_PRINT
%token	 TOKEN_RETURN
%token	 TOKEN_T_STRING
%token	 TOKEN_TRUE
%token	 TOKEN_VOID
%token	 TOKEN_WHILE
%token	 TOKEN_IDENT
%token	 TOKEN_STRING_LITERAL
%token	 TOKEN_CHAR_LITERAL
%token	 TOKEN_INTEGER_LITERAL
%token	 TOKEN_SEMICOLON
%token	 TOKEN_COLON
%token	 TOKEN_COMMA
%token 	 TOKEN_LP
%token	 TOKEN_RP
%token	 TOKEN_LB
%token	 TOKEN_RB
%token	 TOKEN_LCB
%token	 TOKEN_RCB
%token	 TOKEN_INCREMENT
%token	 TOKEN_DECREMENT
%token	 TOKEN_NOT
%token	 TOKEN_EXPONENT
%token 	 TOKEN_MULT
%token   TOKEN_DIV
%token 	 TOKEN_MOD
%token   TOKEN_ADD
%token	 TOKEN_MINUS
%token 	 TOKEN_GT
%token 	 TOKEN_GE
%token 	 TOKEN_LT
%token 	 TOKEN_LE
%token 	 TOKEN_EQ
%token 	 TOKEN_NEQ
%token 	 TOKEN_AND
%token 	 TOKEN_OR
%token 	 TOKEN_ASSIGN
%token	 TOKEN_IDENT_ERROR
%token	 TOKEN_STRING_ERROR
%token	 TOKEN_CHAR_ERROR
%token	 TOKEN_INTEGER_ERROR
%token	 TOKEN_COMMENT_ERROR
%token   TOKEN_SCAN_ERROR

// end here


%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
YYSTYPE is the lexical value returned by each rule in a bison grammar.
By default, it is an integer. In this example, we are returning a pointer to an expression.
*/

//#define YYSTYPE struct expr *

extern char *yytext;
extern int yylex();
extern int yyerror( char *str );

//struct expr * parser_result = 0;
//int parser_result = 0;

%}

%%

/* Here is the grammar: program is the start symbol. */

decl : expr TOKEN_SEMICOLON { return 0; }
	;



expr	: expr TOKEN_ADD term
	| expr TOKEN_MINUS term
	| term
	;

term	: term TOKEN_MULT value
	| term TOKEN_DIV value
	| term TOKEN_MOD value
	| factor
	;

factor  : factor TOKEN_EXPONENT value
	| value
	;

value	: TOKEN_MINUS value
	| TOKEN_LP expr TOKEN_RP
	| TOKEN_INTEGER_LITERAL
	| TOKEN_STRING_LITERAL
	| TOKEN_CHAR_LITERAL
	| TOKEN_TRUE
	| TOKEN_FALSE
	;

%%

/*
This function will be called by bison if the parse should
encounter an error.  In principle, "str" will contain something
useful.  In practice, it often does not.
*/

int yyerror( char *str )
{
	printf("parse error: %s\n",str);
	return 0;
}
