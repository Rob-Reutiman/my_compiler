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

program : stmts 	{ return 0; }
	| 				{ return 0; }
	;

stmts 	:  stmt stmts
	| stmt
	;

stmt: close_if
	| open_if
	;

/*stmt 	: //TOKEN_IF TOKEN_LP expr TOKEN_RP stmt
//	| TOKEN_IF TOKEN_LP expr TOKEN_RP stmt TOKEN_ELSE stmt
	  if_s
	| TOKEN_WHILE TOKEN_LP expr TOKEN_RP stmt
	| TOKEN_FOR TOKEN_LP for_args TOKEN_RP stmt
	| expr TOKEN_SEMICOLON
	| TOKEN_RETURN expr TOKEN_SEMICOLON
	| TOKEN_RETURN TOKEN_SEMICOLON
	| TOKEN_LCB stmts TOKEN_RCB
	| TOKEN_PRINT TOKEN_SEMICOLON;
	| TOKEN_PRINT expr arg_list TOKEN_SEMICOLON
	| decl
	;*/

non_if_stmt:  TOKEN_WHILE TOKEN_LP expr TOKEN_RP stmt
	| TOKEN_FOR TOKEN_LP for_args TOKEN_RP stmt
	| expr TOKEN_SEMICOLON
	| TOKEN_RETURN expr TOKEN_SEMICOLON
	| TOKEN_RETURN TOKEN_SEMICOLON
	| TOKEN_LCB stmts TOKEN_RCB
	| TOKEN_LCB expr arg_list TOKEN_RCB
	| TOKEN_PRINT TOKEN_SEMICOLON;
	| TOKEN_PRINT expr arg_list TOKEN_SEMICOLON
	| decl


open_if : TOKEN_IF TOKEN_LP expr TOKEN_RP stmt
	|  TOKEN_IF TOKEN_LP expr TOKEN_RP close_if TOKEN_ELSE open_if
	;

close_if: non_if_stmt
	| TOKEN_IF TOKEN_LP expr TOKEN_RP close_if TOKEN_ELSE close_if
	;
	

for_args: opt_args TOKEN_SEMICOLON opt_args TOKEN_SEMICOLON opt_args
	;

opt_args: expr
	| 
	;

decl 	: param TOKEN_ASSIGN non_if_stmt 
	| 	param TOKEN_SEMICOLON
	;

param_list 	: TOKEN_COMMA param param_list
	| 
	;

param 	: TOKEN_IDENT TOKEN_COLON type 
	;

expr 	: expr TOKEN_GT subex
	| expr TOKEN_GE subex
	| expr TOKEN_LT subex
	| expr TOKEN_LE subex
	| expr TOKEN_EQ subex
	| expr TOKEN_NEQ subex
	| expr TOKEN_AND subex
	| expr TOKEN_OR subex
	| expr TOKEN_ASSIGN subex
	| expr TOKEN_LB expr TOKEN_RB
	| subex
	;

subex	: subex TOKEN_ADD term
	| subex TOKEN_MINUS term
	| term
	;

term	: term TOKEN_MULT factor
	| term TOKEN_DIV factor
	| term TOKEN_MOD factor
	| factor
	;

factor  : factor TOKEN_EXPONENT value
	| factor TOKEN_INCREMENT 
	| factor TOKEN_DECREMENT
	| value
	;

value	: TOKEN_MINUS value
	| TOKEN_LP expr TOKEN_RP
	| TOKEN_INTEGER_LITERAL
	| TOKEN_STRING_LITERAL
	| TOKEN_CHAR_LITERAL
	| TOKEN_TRUE
	| TOKEN_FALSE
	| TOKEN_IDENT
	| TOKEN_NOT value
	| TOKEN_IDENT TOKEN_LP expr arg_list TOKEN_RP
	| TOKEN_IDENT TOKEN_LP TOKEN_RP
	;

arg_list : TOKEN_COMMA expr arg_list
	| 
	;

type 	: TOKEN_T_ARRAY TOKEN_LB TOKEN_RB type
	| TOKEN_T_ARRAY TOKEN_LB TOKEN_INTEGER_LITERAL TOKEN_RB type
	| TOKEN_T_AUTO
	| TOKEN_T_BOOLEAN
	| TOKEN_T_CHAR
	| TOKEN_T_INTEGER
	| TOKEN_T_STRING
	| TOKEN_VOID
	| TOKEN_FUNCTION type TOKEN_LP param param_list TOKEN_RP
	| TOKEN_FUNCTION type TOKEN_LP TOKEN_RP
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
