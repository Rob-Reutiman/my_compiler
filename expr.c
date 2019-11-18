
#include "expr.h"
#include <stdlib.h>
#include <string.h>

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) {

	struct expr * e = malloc(sizeof(*e));

	e->kind = kind;
	e->left = left;
	e->right = right;

	return e;

}

struct expr * expr_create_name( const char *n ) {

	struct expr *e = expr_create(EXPR_NAME, 0, 0);
	e->name=strdup(n);

	return e;

}

struct expr * expr_create_integer_literal( int c ) {
	
	struct expr *e = expr_create(EXPR_INTEGER_LITERAL, 0, 0);
	e->literal_value=c;

	return e;

}

struct expr * expr_create_boolean_literal( int c ) {

	struct expr *e = expr_create(EXPR_BOOLEAN_LITERAL, 0, 0);
	e->literal_value=c;

	return e;

}

struct expr * expr_create_char_literal( char c ) {

	struct expr *e = expr_create(EXPR_CHAR_LITERAL, 0, 0);
	e->literal_value=c;

	return e;

}

struct expr * expr_create_string_literal( const char *str ) {

	struct expr *e = expr_create(EXPR_STRING_LITERAL, 0, 0);
	e->string_literal=strdup(str);

	return e;

}

// symbol or no

void expr_print( struct expr *e ) {

	if(!e) return;

	switch(e->kind) {
		case EXPR_ADD:
			expr_print(e->left);
			printf("+");
			expr_print(e->right);
			break;
		case EXPR_SUB:
			expr_print(e->left);
			printf("-");
			expr_print(e->right);
			break;
		case EXPR_MUL:
			expr_print(e->left);
			printf("*");
			expr_print(e->right);
			break;
		case EXPR_DIV:
			expr_print(e->left);
			printf("/");
			expr_print(e->right);
			break;
		case EXPR_MOD:
			expr_print(e->left);
			printf("%");
			expr_print(e->right);
			break;
		case EXPR_INCREMENT:
			expr_print(e->left);
			printf("++");
			break;
		case EXPR_DECREMENT:
			expr_print(e->left);
			printf("--");
			break;
		case EXPR_EXPONENT:
			expr_print(e->left);
			printf("^");
			expr_print(e->right);
			break;
		case EXPR_GT:
			expr_print(e->left);
			printf(">");
			expr_print(e->right);
			break;
		case EXPR_GE:
			expr_print(e->left);
			printf(">=");
			expr_print(e->right);
			break;
		case EXPR_LT:
			expr_print(e->left);
			printf("<");
			expr_print(e->right);
			break;
		case EXPR_LE:
			expr_print(e->left);
			printf("<=");
			expr_print(e->right);
			break;
		case EXPR_EQ:
			expr_print(e->left);
			printf("==");
			expr_print(e->right);
			break;
		case EXPR_NEQ:
			expr_print(e->left);
			printf("!=");
			expr_print(e->right);
			break;
		case EXPR_AND:
			expr_print(e->left);
			printf("&&");
			expr_print(e->right);
			break;
		case EXPR_OR:
			expr_print(e->left);
			printf("||");
			expr_print(e->right);
			break;
		case EXPR_ASSIGN:
			expr_print(e->left);
			printf("=");
			expr_print(e->right);
			break;
		case EXPR_ARGLIST:
			expr_print(e->left);
			if(e->right) printf(", ");
			expr_print(e->right);
			break;
		case EXPR_FCALL:
			expr_print(e->left);
			printf("()");	
			break;
		case EXPR_FCALL_ARGS:
			expr_print(e->left);
			printf("(");
			expr_print(e->right);
			printf(")");
			break;
		case EXPR_PAREN:
			expr_print(e->right);
			break;
		case EXPR_REF:
			expr_print(e->left);
			printf("[");
			expr_print(e->right);
			printf("]");
			break;
		case EXPR_NOT:
			printf("!");
			expr_print(e->right);
			break;
		case EXPR_NEG:
			printf("-");
			expr_print(e->right);
			break;
		case EXPR_NAME:
			printf("%s", e->name);
			break;
		case EXPR_BOOLEAN_LITERAL:
			if(e->literal_value == 1) {
				printf("true");
			} else {
				printf("false");
			}
			break;
		case EXPR_CHAR_LITERAL:
			printf("\'");
			char_print(e->literal_value);
			printf("\'");
			break;
		case EXPR_INTEGER_LITERAL:
			printf("%d", e->literal_value);
			break;
		case EXPR_STRING_LITERAL:
			printf("\"");
			string_print(e->string_literal);
			printf("\"");
			break;
		default:
			break;
	}

}

void string_print(const char* c) {

	while(*c) {
		char_print(*c);
		c++;
	}

};

void char_print(char c) {

	switch(c) {
		case '\a': printf("\\a"); break;
			
		case '\b': printf("\\b"); break;

		case '\e': printf ("\\e"); break;

		case '\f': printf ("\\f"); break;

		case '\n': printf ("\\n"); break;

		case '\r': printf("\\r"); break;

		case '\t': printf("\\t"); break;

		case '\v': printf("\\v"); break;

		case '\\': printf("\\\\"); break;

		case '\'': printf("\\\'"); break;

		case '\"': printf("\\\""); break;
                      
		default: printf("%c", c); break;
	}
}

void expr_resolve( struct expr *e, struct hash_table *h) {

	if(!e) return;

	if( e->kind == EXPR_NAME ) {
		e->symbol = scope_lookup(h, e->name);
		if(e->symbol == NULL) {
			fprintf(stderr, "resolve error: %s is not defined\n", e->name);
		}
	} else {
		expr_resolve( e->left, h );
		expr_resolve( e->right, h);
	}	

}

void expr_delete(struct expr * e ) {

	if(!e) return;
	expr_delete(e->left);
	expr_delete(e->right);

	symbol_delete(e->symbol);

	free(e);

}
