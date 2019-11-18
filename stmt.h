
#ifndef STMT_H
#define STMT_H

#include "decl.h"
#include "expr.h"
#include "param_list.h"

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
};

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next );
void stmt_print( struct stmt *s, int indent );
void tab_print(int ident);
void stmt_resolve( struct stmt *s, struct hash_table *h);
void stmt_typecheck( struct stmt *s);
void stmt_delete( struct stmt *s );


#endif
