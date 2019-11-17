#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"

typedef enum {
	EXPR_ADD=0,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_INCREMENT,
	EXPR_DECREMENT,
	EXPR_EXPONENT,
	EXPR_GT,
	EXPR_GE,
	EXPR_LT,
	EXPR_LE,
	EXPR_EQ,
	EXPR_NEQ,
	EXPR_AND,
	EXPR_OR,
	EXPR_ASSIGN,
	// base types - symbol??
	EXPR_ARGLIST,
	EXPR_FCALL,
	EXPR_FCALL_ARGS,
	EXPR_PAREN,
	EXPR_REF,
	EXPR_NOT,
	EXPR_NEG,
	EXPR_NAME,
	EXPR_BOOLEAN_LITERAL,
	EXPR_CHAR_LITERAL,
	EXPR_INTEGER_LITERAL,
	EXPR_STRING_LITERAL
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int literal_value;
	const char * string_literal;
	struct symbol *symbol;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( char c );
struct expr * expr_create_string_literal( const char *str );

void expr_print( struct expr *e );
void string_print( const char* string_literal );
void char_print(char c);
void expr_delete( struct expr *e );

#endif
