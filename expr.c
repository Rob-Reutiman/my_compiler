
#include "expr.h"

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) {

	struct expr * e = calloc((size_t)1, sizeof(e));

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
	e->string_literal=strdup(c);

	return e;

}
struct expr * expr_create_string_literal( const char *str ) {

	struct expr *e = expr_create(EXPR_STRING_LITERAL, 0, 0);
	e->string_literal=strdup(str);

	return e;

}

// symbol or no

void expr_print( struct expr *e ) {

	switch(e->kind) {
		case EXPR_ADD:
		// surely there must be a better way to do this
	}

}

void expr_delete(struct expr * e ) {

	if(!e) return;
	expr_delete(e->left);
	expr_delete(e-<right);

	if(e->name) {
		free(e->name);	
	}

	if(e->string_literal) {
		free(e->string_literal);
	}

	symbol_delete(e->symbol);

	free(e);

}
