/*
Declare token types at the top of the bison file,
causing them to be automatically generated in parser.tab.h
for use by scanner.c.
*/

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
	| TOKEN_PRINT arg_list TOKEN_SEMICOLON
	| decl
	;*/

non_if_stmt:  TOKEN_WHILE TOKEN_LP expr TOKEN_RP stmt
	| TOKEN_FOR TOKEN_LP for_args TOKEN_RP stmt
	| expr TOKEN_SEMICOLON
	| TOKEN_RETURN expr TOKEN_SEMICOLON
	| TOKEN_RETURN TOKEN_SEMICOLON
	| TOKEN_LCB stmts TOKEN_RCB
	| TOKEN_LCB arg_list TOKEN_RCB
	| TOKEN_PRINT TOKEN_SEMICOLON;
	| TOKEN_PRINT arg_list TOKEN_SEMICOLON
	| decl


open_if : TOKEN_IF TOKEN_LP expr TOKEN_RP stmt
	|  TOKEN_IF TOKEN_LP expr TOKEN_RP close_if TOKEN_ELSE open_if
	;

close_if: non_if_stmt
	| TOKEN_IF TOKEN_LP expr TOKEN_RP close_if TOKEN_ELSE close_if
	;
	
// for loop args

for_args: opt_args TOKEN_SEMICOLON opt_args TOKEN_SEMICOLON opt_args
	;

opt_args: expr
	| 
	;

// decl

decl 	: param TOKEN_ASSIGN non_if_stmt 
	| 	param TOKEN_SEMICOLON
	;

// param_list

param_list 	: param
	| param TOKEN_COMMA param_list
	;

param 	: TOKEN_IDENT TOKEN_COLON type 
	;

// All types of exprs begin here

expr 	:  expr TOKEN_ASSIGN layer9
	| layer9
	;

layer9 	: layer9 TOKEN_OR layer8
	| layer8
	;

layer8 	: layer8 TOKEN_AND layer7
	| layer7
	;

layer7 	: layer7 TOKEN_GT layer6
	| layer7 TOKEN_GE layer6
	| layer7 TOKEN_LT layer6
	| layer7 TOKEN_LE layer6
	| layer7 TOKEN_EQ layer6
	| layer7 TOKEN_NEQ layer6
	| layer6
	;

layer6	: layer6 TOKEN_ADD layer5
	| layer6 TOKEN_MINUS layer5
	| layer5
	;

layer5	: layer5 TOKEN_MULT layer4
	| layer5 TOKEN_DIV layer4
	| layer5 TOKEN_MOD layer4
	| layer4
	;

layer4 	: TOKEN_EXPONENT layer3
	| layer3
	;

layer3 	: TOKEN_NOT layer2
	| TOKEN_MINUS layer2
	| layer2
	;

layer2  : layer2 TOKEN_INCREMENT 
	| layer2 TOKEN_DECREMENT
	| layer1
	;

layer1 	: TOKEN_IDENT TOKEN_LP arg_list TOKEN_RP
	| TOKEN_IDENT TOKEN_LP TOKEN_RP
	| TOKEN_LP expr TOKEN_RP
	| TOKEN_IDENT TOKEN_LB expr TOKEN_RB
	| value
	;

value	: TOKEN_INTEGER_LITERAL
	| TOKEN_STRING_LITERAL
	| TOKEN_CHAR_LITERAL
	| TOKEN_TRUE
	| TOKEN_FALSE
	| TOKEN_IDENT
	;

// arg_list

arg_list : expr
	| expr TOKEN_COMMA arg_list
	;

// types

type 	: TOKEN_T_ARRAY TOKEN_LB TOKEN_RB type
	| TOKEN_T_ARRAY TOKEN_LB TOKEN_INTEGER_LITERAL TOKEN_RB type
	| TOKEN_T_AUTO
	| TOKEN_T_BOOLEAN
	| TOKEN_T_CHAR
	| TOKEN_T_INTEGER
	| TOKEN_T_STRING
	| TOKEN_VOID
	| TOKEN_FUNCTION type TOKEN_LP param_list TOKEN_RP
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
