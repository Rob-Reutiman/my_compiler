
#include "stmt.h"

// delete from here
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

// to here

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ) {

	struct stmt *s = calloc((size_t)1, sizeof(stmt));

	s->kind = kind;
	s->decl = decl;
	// if, for, while?
	s->int_expr = init_expr;
	s->expr = expr;
	s->next_expr = next_expr;

	s->body = body;
	// if an else exists
	s->else_body = else_body;

	s->next = next;

}

void stmt_print( struct stmt *s, int indent ) {

	while(indent > 0) {
		printf("\t");
		ident--;
	}

	switch(s->kind) {
		case STMT_DECL:
			decl_print(s->decl, 0);
			break;
		case STMT_EXPR:
			expr_print(s->expr, 0);
			break;
		case STMT_IF_ELSE: 
			printf(
			break;
		case STMT_FOR:
			
			break;
		case STMT_PRINT:
			
			break;
		case STMT_RETURN:

			break;
		case STMT_BLOCK:

			break;
		case default:

			break;

	}	

}

void stmt_delete(struct stmt *s ) {
	if(!s) return;

	decl_delete(s->decl);
	expr_delete(s->init_expr);
	expr_delete(s->expr);
	expr_delete(s->next_expr);
	stmt_delete(s->body);
	stmt_delete(s->else_body);
	stmt_delete(s->next);

	free(s);
}
