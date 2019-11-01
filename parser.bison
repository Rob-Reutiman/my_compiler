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

%union { 
	struct decl *decl;
	struct stmt *stmt;
	struct expr *expr;
	struct type *type;
	struct param_list *param_list;
//	struct symbol *symbol; 
};

%type <decl> decl
%type <stmt> stmts stmt non_if_stmt open_if restricted
%type <expr> expr expr_or expr_and expr_comp expr_add expr_mult expr_expo expr_not expr_postfix expr_groups value arg_list ident opt_args 
%type <type> type
%type <param_list> param_list param
// %type <symbol>

%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "type.h"
#include "decl.h"


extern char *yytext;
extern int yylex();
extern int yyerror( char *str );

%}

%%

/* Here is the grammar: program is the start symbol. */

program : stmts 	{ return 0; }
	| 				{ return 0; }
	;

// stmts

stmts 	:  stmt stmts
	| stmt
	;

stmt: open_if
	| restricted
	;

// if else problem handlers

open_if : TOKEN_IF TOKEN_LP expr TOKEN_RP stmt
			{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, NULL, NULL); }
	|  TOKEN_IF TOKEN_LP expr TOKEN_RP restricted TOKEN_ELSE open_if
			{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, $7, NULL); }
	;

restricted: non_if_stmt
	| TOKEN_IF TOKEN_LP expr TOKEN_RP restricted TOKEN_ELSE restricted
			{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, $7, NULL); }
	;

// non_if_statements

non_if_stmt:  TOKEN_WHILE TOKEN_LP expr TOKEN_RP stmt
			{ $$ = NULL; }
	| TOKEN_FOR TOKEN_LP opt_args TOKEN_SEMICOLON opt_args TOKEN_SEMICOLON opt_args TOKEN_RP stmt
			{ $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9, NULL, NULL); }
	| expr TOKEN_SEMICOLON
			{ $$ = stmt_create(STMT_EXPR, NULL, NULL, NULL, $1, NULL, NULL, NULL); }
	| TOKEN_RETURN expr TOKEN_SEMICOLON
			{ $$ = stmt_create(STMT_RETURN, NULL, NULL, NULL, $2, NULL, NULL, NULL); }
	| TOKEN_RETURN TOKEN_SEMICOLON
			{ $$ = stmt_create(STMT_RETURN, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
	| TOKEN_LCB stmts TOKEN_RCB
			{ $$ = $2; }
	| TOKEN_LCB arg_list TOKEN_RCB
			{ $$ = $2; }
	| TOKEN_PRINT TOKEN_SEMICOLON
			{ $$ = stmt_create(STMT_PRINT, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
	| TOKEN_PRINT arg_list TOKEN_SEMICOLON
			{ $$ = stmt_create(STMT_PRINT, NULL, NULL, NULL, $2, NULL, NULL, NULL); }
	| decl
			{ $$ = stmt_create(STMT_DECL, $1, NULL, NULL, NULL, NULL, NULL, NULL); }
	;
	
// for loop args

opt_args: expr
	| { $$ = NULL; } 		
	;

// decl

decl 	: param TOKEN_ASSIGN non_if_stmt 
			{ $$ = decl_create($1->name, $1->type, NULL, $3, NULL); }
	| 	param TOKEN_SEMICOLON
			{ $$ = decl_create($1->name, $1->type, NULL, NULL, NULL); }
	;

// param_list

param_list 	: param
	| param TOKEN_COMMA param_list
			{ $1->next = $3; }
	;

param 	:  ident TOKEN_COLON type 
			{ $$ = param_list_create(strdup($1->name), $3, NULL); }
	;

// All types of exprs begin here

expr 	:  expr TOKEN_ASSIGN expr_or
			{ $$ = expr_create(EXPR_ASSIGN, $1 , $3 ); }
	| expr_or
	;

expr_or 	: expr_or TOKEN_OR expr_and
			{ $$ = expr_create(EXPR_OR, $1 , $3 ); }
	| expr_and
	;

expr_and 	: expr_and TOKEN_AND expr_comp
			{ $$ = expr_create(EXPR_AND, $1 , $1 ); }
	| expr_comp
	;

expr_comp 	: expr_comp TOKEN_GT expr_add
			{ $$ = expr_create(EXPR_GT, $1, $3 ); }
	| expr_comp TOKEN_GE expr_add
			{ $$ = expr_create(EXPR_GE, $1, $3 ); }
	| expr_comp TOKEN_LT expr_add
			{ $$ = expr_create(EXPR_LT, $1, $3 ); }
	| expr_comp TOKEN_LE expr_add
			{ $$ = expr_create(EXPR_LE, $1, $3 ); }
	| expr_comp TOKEN_EQ expr_add
			{ $$ = expr_create(EXPR_EQ, $1, $3 ); }
	| expr_comp TOKEN_NEQ expr_add
			{ $$ = expr_create(EXPR_NEQ, $1, $3 ); }
	| expr_add
	;

expr_add	: expr_add TOKEN_ADD expr_mult
			{ $$ = expr_create(EXPR_ADD, $1, $3 ); }
	| expr_add TOKEN_MINUS expr_mult
			{ $$ = expr_create(EXPR_SUB, $1, $3 ); }
	| expr_mult
	;

expr_mult	: expr_mult TOKEN_MULT expr_expo
			{ $$ = expr_create(EXPR_MUL , $1 , $3 ); }
	| expr_mult TOKEN_DIV expr_expo
			{ $$ = expr_create(EXPR_DIV, $1, $3 ); }
	| expr_mult TOKEN_MOD expr_expo
			{ $$ = expr_create(EXPR_MOD, $1, $3 ); }
	| expr_expo
	;

expr_expo 	: expr_expo TOKEN_EXPONENT expr_not
			{ $$ = expr_create(EXPR_EXPONENT, $1, $3 ); }
	| expr_not
	;

expr_not 	: TOKEN_NOT expr_postfix
			{ $$ = expr_create(EXPR_NOT, NULL, $2 ); }
	| TOKEN_MINUS expr_postfix
			{ $$ = expr_create(EXPR_NEG, NULL, $2 ); }
	| expr_postfix
	;

expr_postfix  : expr_postfix TOKEN_INCREMENT
			{ $$ = expr_create(EXPR_INCREMENT, $1, NULL ); }
	| expr_postfix TOKEN_DECREMENT
			{ $$ = expr_create(EXPR_DECREMENT, $1, NULL ); }
	| expr_groups
	;

expr_groups 	: ident TOKEN_LP arg_list TOKEN_RP
			{ $$ = expr_create(EXPR_FCALL_ARGS, $1, $3 ); }
	| ident TOKEN_LP TOKEN_RP
			{ $$ = expr_create(EXPR_FCALL, $1, NULL ); }
	| TOKEN_LP expr TOKEN_RP
			{ $$ = expr_create(EXPR_PAREN, NULL, $2 ); }
	| ident TOKEN_LB expr TOKEN_RB
			{ $$ = expr_create(EXPR_REF, $1, $3 ); }
	| value
	;

value	: TOKEN_INTEGER_LITERAL
			{ $$ = expr_create_integer_literal(atoi(yytext)); }
	| TOKEN_STRING_LITERAL
			{ $$ = expr_create_string_literal(yytext); }
	| TOKEN_CHAR_LITERAL
			{ $$ = expr_create_char_literal(yytext[0]); }
	| TOKEN_TRUE
			{ $$ = expr_create_boolean_literal(true); }
	| TOKEN_FALSE
			{ $$ = expr_create_boolean_literal(false); }
	| ident 
	;

ident 	: TOKEN_IDENT
			{ $$ = expr_create_name(yytext); }
	;

// arg_list

arg_list : expr
	| expr TOKEN_COMMA arg_list
	;

// types

type 	: TOKEN_T_ARRAY TOKEN_LB TOKEN_RB type
			{ $$ = type_create(TYPE_ARRAY, $4, 0 ); }
	| TOKEN_T_ARRAY TOKEN_LB TOKEN_INTEGER_LITERAL TOKEN_RB type
			{ $$ = type_create(TYPE_ARRAY, $5, 0 ); }
	| TOKEN_T_AUTO
			{ $$ = type_create(TYPE_AUTO, 0, 0 ); }
	| TOKEN_T_BOOLEAN
			{ $$ = type_create(TYPE_BOOLEAN, 0, 0 ); }
	| TOKEN_T_CHAR
			{ $$ = type_create(TYPE_CHARACTER, 0, 0 ); }
	| TOKEN_T_INTEGER
			{ $$ = type_create(TYPE_INTEGER, 0, 0 ); }
	| TOKEN_T_STRING
			{ $$ = type_create(TYPE_STRING, 0, 0 ); }
	| TOKEN_VOID
			{ $$ = type_create(TYPE_VOID, 0, 0 ); }
	| TOKEN_FUNCTION type TOKEN_LP param_list TOKEN_RP
			{ $$ = type_create(TYPE_FUNCTION, $2, $4 ); }
	| TOKEN_FUNCTION type TOKEN_LP TOKEN_RP
			{ $$ = type_create(TYPE_FUNCTION, $2, NULL ); }
	;

%%

/*
This function will be called by bison if the parse should
encounter an error.  In principle, "str" will contain something
useful.  In practice, it often does not.
*/

int yyerror( char *str )
{
	printf("parse error: %s\n", str);
	return 1;
}
