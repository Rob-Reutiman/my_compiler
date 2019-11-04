
#include "stmt.h"
#include <stdlib.h>

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ) {

	struct stmt * s = malloc(sizeof(struct stmt));

	s->kind = kind;
	s->decl = decl;
	s->init_expr = init_expr;
	s->expr = expr;
	s->next_expr = next_expr;
	s->body = body;
	s->else_body = else_body;
	s->next = next;

	return s;
}

void stmt_print( struct stmt *s, int indent ) {

	if(!s) return;	

	switch(s->kind) {
		case STMT_DECL:
			decl_print(s->decl, indent);
			break;
		case STMT_EXPR:
			expr_print(s->expr);
			printf(";\n");
			break;
		case STMT_IF_ELSE: 
			tab_print(indent);
			printf("if(");
			expr_print(s->expr);
			printf(") {\n");
			stmt_print(s->body, indent+1);
			printf("\n");
			tab_print(indent);
			printf("}");
			if(s->else_body) {
				printf(" else {\n");
				stmt_print(s->else_body, indent+1);
				printf("\n");
				tab_print(indent);
				printf("}");
			}
			break;
		case STMT_FOR:
			tab_print(indent);
			printf("for(");
			expr_print(s->init_expr);
			printf(";");
			expr_print(s->expr);
			printf(";");
			expr_print(s->next_expr);
			printf(";) {\n");
			stmt_print(s->body, indent+1);
			printf("}\n");
			break;
		case STMT_PRINT:
			tab_print(indent);
			printf("print ");
			expr_print(s->expr);
			printf(";");
			break;
		case STMT_RETURN:
			tab_print(indent);
			printf("return ");
			expr_print(s->expr);
			printf(";");
			break;
		case STMT_BLOCK:
			stmt_print(s->body, indent);
			stmt_print(s->next, indent);
			break;
		default:
			break;

	}	

}

void tab_print(int indent) {

	int temp = indent;

	while(temp > 0) {
		printf("\t");
		temp--;
	}

	return;

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

