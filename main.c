  
#include "token.h"
#include <stdio.h>
#include <string.h>

extern FILE *yyin;
extern int yylex();
extern char *yytext;

char* get_TOKEN_TYPE(int t_num) {

	switch(t_num) {
	
		case TOKEN_EOF:
			return "EOF";
			break;

		case TOKEN_T_ARRAY:
			return "ARRAY";
			break;

		case TOKEN_T_AUTO:
			return "AUTO";
			break;

		case TOKEN_T_BOOLEAN:
			return "BOOLEAN";
			break;

		case TOKEN_T_CHAR:
			return "CHAR";
			break;

		case TOKEN_ELSE:
			return "ELSE";
			break;

		case TOKEN_FALSE:
			return "FALSE";
			break;

		case TOKEN_FOR:
			return "FOR";
			break;

		case TOKEN_FUNCTION:
			return "FUNCTION";
			break;

		case TOKEN_IF:
			return "IF";
			break;

		case TOKEN_T_INTEGER:
			return "INTEGER";
			break;

		case TOKEN_PRINT:
			return "PRINT";
			break;

		case TOKEN_RETURN:
			return "RETURN";
			break;

		case TOKEN_T_STRING:
			return "STRING";
			break;

		case TOKEN_TRUE:
			return "TRUE";
			break;

		case TOKEN_VOID:
			return "VOID";
			break;

		case TOKEN_WHILE:
			return "WHILE";
			break;

		case TOKEN_IDENT:
			return "IDENT";
			break;

		case TOKEN_STRING_LITERAL:
			return "STRING_LITERAL";
			break;

		case TOKEN_CHAR_LITERAL:
			return "CHAR_LITERAL";
			break;

		case TOKEN_INTEGER_LITERAL:
			return "INTEGER_LITERAL";
			break;

		case TOKEN_SEMICOLON:
			return "SEMICOLON";
			break;

		case TOKEN_COLON:
			return "COLON";
			break;

		case TOKEN_COMMA:
			return "COMMA";
			break;

		case TOKEN_LP:
			return "LP";
			break;

		case TOKEN_RP:
			return "RP";
			break;

		case TOKEN_LB:
			return "LB";
			break;

		case TOKEN_RB:
			return "RB";
			break;

		case TOKEN_LCB:
			return "LCB";
			break;

		case TOKEN_RCB:
			return "RCB";
			break;

		case TOKEN_INCREMENT:
			return "INCREMENT";
			break;

		case TOKEN_DECREMENT:
			return "DECREMENT";
			break;

		case TOKEN_NOT:
			return "NOT";
			break;

		case TOKEN_EXPONENT:
			return "EXPONENT";
			break;

		case TOKEN_MULT:
			return "MULT";
			break;

		case TOKEN_DIV:
			return "DIV";
			break;

		case TOKEN_MOD:
			return "MOD";
			break;

		case TOKEN_ADD:
			return "ADD";
			break;

		case TOKEN_MINUS:
			return "MINUS";
			break;

		case TOKEN_GT:
			return "GT";
			break;

		case TOKEN_GE:
			return "GE";
			break;

		case TOKEN_LT:
			return "LT";
			break;

		case TOKEN_LE:
			return "LE";
			break;

		case TOKEN_EQ:
			return "EQ";
			break;

		case TOKEN_NEQ:
			return "NEQ";
			break;

		case TOKEN_AND:
			return "AND";
			break;

		case TOKEN_OR:
			return "OR";
			break;

		case TOKEN_ASSIGN:
			return "ASSIGN";
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
			if(t==TOKEN_IDENT || t==TOKEN_STRING_LITERAL || t==TOKEN_CHAR_LITERAL || t==TOKEN_INTEGER_LITERAL) {
				printf("%s %s\n", get_TOKEN_TYPE(t), yytext);
			} else {
				printf("%s\n", get_TOKEN_TYPE(t));
			}
		}
	} else {
		printf("No -scan flag passed\n");
		return 1;
	}

	return 0;
}
