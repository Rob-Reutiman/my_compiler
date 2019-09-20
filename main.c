  
#include "token.h"
#include <stdio.h>

extern FILE *yyin;
extern int yylex();
extern char *yytext;

char* get_TOKEN_TYPE(int t_num) {

	switch(t_num) {
	
		case 0:
			return "EOF";
			break;

		case 1:
			return "IDENT";
			break;

		case 2:
			return "ARRAY";
			break;

		case 3:
			return "AUTO";
			break;

		case 4:
			return "BOOLEAN";
			break;

		case 5:
			return "CHAR";
			break;

		case 6:
			return "ELSE";
			break;

		case 7:
			return "FALSE";
			break;

		case 8:
			return "FOR";
			break;

		case 9:
			return "FUNCTION";
			break;

		case 10:
			return "IF";
			break;

		case 11:
			return "INTEGER";
			break;

		case 12:
			return "PRINT";
			break;

		case 13:
			return "RETURN";
			break;

		case 14:
			return "STRING";
			break;

		case 15:
			return "TRUE";
			break;

		case 16:
			return "VOID";
			break;

		case 17:
			return "WHILE";
			break;

		case 18:
			return "INCREMENT";
			break;

		case 19:
			return "ADD";
			break;

		case 20:
			return "COMMENT";
			break;

		case 21:
			return "STRING_LITERAL";
			break;

		case 22:
			return "INTEGER_LITERAL";
			break;

		case 23:
			return "SEMICOLON";
			break;

		case 24:
			return "CHAR_LITERAL";
			break;

		case 25:
			return "NOT";
			break;

		case 26:
			return "EXPONENT";
			break;

		case 27:
			return "MULT";
			break;

		case 28:
			return "DIV";
			break;

		case 29:
			return "MOD";
			break;

		case 30:
			return "GT";
			break;

		case 31:
			return "GE";
			break;

		case 32:
			return "GE";
			break;

		case 33:
			return "LE";
			break;

		case 34:
			return "EQ";
			break;

		case 35:
			return "NEQ";
			break;

		case 36:
			return "AND";
			break;

		case 37:
			return "OR";
			break;

		case 38:
			return "ASSIGN";
			break;

		case 39:
			return "DECREMENT";
			break;

		case 40:
			return "MINUS";
			break;

		case 41:
			return "COLON";
			break;

		case 42:
			return "LP";
			break;

		case 43:
			return "RP";
			break;

		case 44:
			return "LB";
			break;

		case 45:
			return "RB";
			break;

		case 46:
			return "LCB";
			break;

		case 47:
			return "RCB";
			break;

		case 48:
			return "COMMA";
			break;

		default:
			return "ERROR";
			break;
		//	exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[])
{
	int argind=1;
	int SCAN = 0;

	while (argind < argc && argv[argind][0] == '-') {
		if(!strcmp(argv[argind++], "-scan"))
			SCAN = 1;
	}

	if(SCAN) {
		yyin = fopen(argv[argind],"r");
		if(!yyin) {
			printf("could not open %s\n", argv[argind]);
			return 1;
		}

		while(1) {
			token_t t = yylex();
			if(t==TOKEN_EOF) break;
			printf("%s %s\n", get_TOKEN_TYPE(t), yytext);
		}
	} else {
		printf("No -scan flag passed\n");
		return 1;
	}

	return 0;
}
