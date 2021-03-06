  
#include "token.h"
#include "scratch.h"
#include "hash_table.h"
#include "scope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern char *yytext;
extern struct stmt * parser_result;
char* get_TOKEN_TYPE(token_t);
int RESOLVE_ERROR = 1;
int TYPE_ERROR = 1;
int SCRATCH_TABLE[6] = {0};
int COUNTER = 0;

typedef enum {
	SCAN=1,
	PARSE,
	PRINT,
	RESOLVE,
	TYPECHECK,
	CODEGEN
} flag_t;

void flag_set(flag_t*, char*);

int main(int argc, char* argv[]) {

	flag_t flag = 0;

	flag_set(&flag, argv[1]);

	if(argc == 3 || argc == 4) {

		yyin = fopen(argv[2],"r");
		if(!yyin) {
			printf("could not open %s\n", argv[2]);
			return 1;
		}

		switch(flag) {
			case SCAN: 
				if(argc != 3) break;
				while(1) {
					token_t t = yylex();
					if(t==TOKEN_EOF) break;
					if(t==TOKEN_IDENT || t==TOKEN_STRING_LITERAL || t==TOKEN_CHAR_LITERAL || t==TOKEN_INTEGER_LITERAL) {
						printf("%s %s\n", get_TOKEN_TYPE(t), yytext);
					} else {
						printf("%s\n", get_TOKEN_TYPE(t));
					}
				}
				break;

			case PARSE: 
				if(argc != 3) break;
				if(yyparse()==0) { 
					printf("parse successful\n");
					return 0;
				} else {
					fprintf(stderr, "parse failed!\n");
					return 1;
				}  
				break;
			

			case PRINT:
				if(argc != 3) break;
				if(yyparse()==0) { 
					stmt_print(parser_result, 0);
					return 0;
				} else {
					fprintf(stderr, "parse failed!\n");
					return 1;
				}  
				break;

			case RESOLVE:
				if(argc != 3) break;
				if(yyparse()==0) { 
					struct hash_table *h = NULL;
					stmt_resolve(parser_result, h);
					if(RESOLVE_ERROR == 0) {
						return 1;
					}
					return 0;
				} else {
					fprintf(stderr, "parse failed!\n");
					return 1;
				}  
				break;

			case TYPECHECK:
				if(argc != 3) break;
				if(yyparse()==0) { 
					struct hash_table *h = NULL;
					stmt_resolve(parser_result, h);
					if(RESOLVE_ERROR == 0) {
						return 1;
					}
					stmt_typecheck(parser_result);
					if(TYPE_ERROR == 0) {
						return 1;
					}
					return 0;
				} else {
					fprintf(stderr, "parse failed!\n");
					return 1;
				}  
				break;

			case CODEGEN:
				if(argc != 4) break;
				if(yyparse()==0) { 
					struct hash_table *h = NULL;
					stmt_resolve(parser_result, h);
					if(RESOLVE_ERROR == 0) {
						return 1;
					}
					stmt_typecheck(parser_result);
					if(TYPE_ERROR == 0) {
						return 1;
					}
					FILE* output = fopen(argv[3], "w");
					stmt_codegen(parser_result, output);
					fclose(output);
					return 0;

				} else {
					fprintf(stderr, "parse failed!\n");
					return 1;
				}  
				break;

			default:
				fprintf(stderr, "Usage: Invalid flag\n");
				return 1;
		}

	} 
	
	fprintf(stderr, "Usage: bminor -[flag] [sourcefile.bminor] [assemblyfile.s]\n");
	return 1;

}

char* get_TOKEN_TYPE(token_t t_num) {

	switch(t_num) {
	
		case TOKEN_EOF:
			return "EOF";

		case TOKEN_T_ARRAY:
			return "ARRAY";
		
		case TOKEN_T_AUTO:
			return "AUTO";

		case TOKEN_T_BOOLEAN:
			return "BOOLEAN";

		case TOKEN_T_CHAR:
			return "CHAR";

		case TOKEN_ELSE:
			return "ELSE";

		case TOKEN_FALSE:
			return "FALSE";

		case TOKEN_FOR:
			return "FOR";

		case TOKEN_FUNCTION:
			return "FUNCTION";

		case TOKEN_IF:
			return "IF";

		case TOKEN_T_INTEGER:
			return "INTEGER";

		case TOKEN_PRINT:
			return "PRINT";

		case TOKEN_RETURN:
			return "RETURN";

		case TOKEN_T_STRING:
			return "STRING";

		case TOKEN_TRUE:
			return "TRUE";

		case TOKEN_VOID:
			return "VOID";

		case TOKEN_WHILE:
			return "WHILE";

		case TOKEN_IDENT:
			return "IDENT";

		case TOKEN_STRING_LITERAL:
			return "STRING_LITERAL";

		case TOKEN_CHAR_LITERAL:
			return "CHAR_LITERAL";

		case TOKEN_INTEGER_LITERAL:
			return "INTEGER_LITERAL";

		case TOKEN_SEMICOLON:
			return "SEMICOLON";

		case TOKEN_COLON:
			return "COLON";

		case TOKEN_COMMA:
			return "COMMA";

		case TOKEN_LP:
			return "LP";

		case TOKEN_RP:
			return "RP";

		case TOKEN_LB:
			return "LB";

		case TOKEN_RB:
			return "RB";

		case TOKEN_LCB:
			return "LCB";

		case TOKEN_RCB:
			return "RCB";
			break;

		case TOKEN_INCREMENT:
			return "INCREMENT";

		case TOKEN_DECREMENT:
			return "DECREMENT";

		case TOKEN_NOT:
			return "NOT";

		case TOKEN_EXPONENT:
			return "EXPONENT";

		case TOKEN_MULT:
			return "MULT";

		case TOKEN_DIV:
			return "DIV";

		case TOKEN_MOD:
			return "MOD";

		case TOKEN_ADD:
			return "ADD";

		case TOKEN_MINUS:
			return "MINUS";

		case TOKEN_GT:
			return "GT";

		case TOKEN_GE:
			return "GE";

		case TOKEN_LT:
			return "LT";

		case TOKEN_LE:
			return "LE";

		case TOKEN_EQ:
			return "EQ";

		case TOKEN_NEQ:
			return "NEQ";

		case TOKEN_AND:
			return "AND";

		case TOKEN_OR:
			return "OR";

		case TOKEN_ASSIGN:
			return "ASSIGN";

		case TOKEN_IDENT_ERROR:
			fprintf(stderr, "scan error: invalid identifier %s - max length is 256 characters\n", yytext);
			exit(1);

		case TOKEN_STRING_ERROR:
			fprintf(stderr, "scan error: invalid string %s - max length is 255 characters\n", yytext);
			exit(1);

		case TOKEN_CHAR_ERROR:
			fprintf(stderr, "scan error: invalid char: %s\n", yytext);
			exit(1);

		case TOKEN_INTEGER_ERROR:
			fprintf(stderr, "scan error: invalid integer %s - must fit in 64 bits\n", yytext);
			exit(1);

		case TOKEN_COMMENT_ERROR:
			fprintf(stderr, "scan error: invalid comment syntax %s\n", yytext);
			exit(1);

		case TOKEN_SCAN_ERROR:
			fprintf(stderr, "scan error: %s is not a valid character\n", yytext);
			exit(1);

		default:
			fprintf(stderr, "scan error: unknown token %s\n", yytext);
			exit(1);
	}
}

void flag_set(flag_t *flag, char* str) {
	if(!strcmp(str, "-scan")) {
		*flag = SCAN;
	}

	if(!strcmp(str, "-parse")) {
		*flag = PARSE;
	}

	if(!strcmp(str, "-print")) {
		*flag = PRINT;
	}

	if(!strcmp(str, "-resolve")) {
		*flag = RESOLVE;
	}

	if(!strcmp(str, "-typecheck")) {
		*flag = TYPECHECK;
	}

	if(!strcmp(str, "-codegen")) {
		*flag = CODEGEN;
	}
}
