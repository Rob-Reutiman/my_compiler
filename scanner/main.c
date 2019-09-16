  
#include "token.h"
#include <stdio.h>

extern FILE *yyin;
extern int yylex();
extern char *yytext;

char* get_TOKEN_TYPE(int t_num) {

	switch(t_num) {
	
		case 0:
			return "TOKEN_EOF";
			break;

		case 1:
			return "TOKEN_IDENT";
			break;

		case 2:
			return "TOKEN_T_ARRAY";
			break;

		case 3:
			return "TOKEN_T_AUTO";
			break;

		case 4:
			return "TOKEN_T_BOOLEAN";
			break;

		case 5:
			return "TOKEN_T_CHAR";
			break;

		case 6:
			return "TOKEN_ELSE";
			break;

		case 7:
			return "TOKEN_FALSE";
			break;

		case 8:
			return "TOKEN_FOR";
			break;

		case 9:
			return "TOKEN_FUNCTION";
			break;

		case 10:
			return "TOKEN_IF";
			break;

		case 11:
			return "TOKEN_T_INTEGER";
			break;

		case 12:
			return "TOKEN_PRINT";
			break;

		case 13:
			return "TOKEN_RETURN";
			break;

		case 14:
			return "TOKEN_T_STRING";
			break;

		case 15:
			return "TOKEN_TRUE";
			break;

		case 16:
			return "TOKEN_VOID";
			break;

		case 17:
			return "TOKEN_WHILE";
			break;

		case 18:
			return "TOKEN_INCREMENT";
			break;

		case 19:
			return "TOKEN_ADD";
			break;

		default:
			return "TOKEN_ERROR";
			break;
		//	exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[])
{
	yyin = fopen(argv[1],"r");
	if(!yyin) {
		printf("could not open program.c!\n");
		return 1;
	}

	while(1) {
		token_t t = yylex();
		if(t==TOKEN_EOF) break;
		printf("token: %s  text: %s\n", get_TOKEN_TYPE(t), yytext);
	}
}
