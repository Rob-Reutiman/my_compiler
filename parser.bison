/*
Declare token types at the top of the bison file,
causing them to be automatically generated in parser.tab.h
for use by scanner.c.
*/

%token TOKEN_ID


%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
YYSTYPE is the lexical value returned by each rule in a bison grammar.
By default, it is an integer. In this example, we are returning a pointer to an expression.
*/

extern char *yytext;
extern int yylex();
extern int yyerror( char *str );

//struct expr * parser_result = 0;

%}

%%

/* Here is the grammar: program is the start symbol. */

program : TOKEN_ID
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
