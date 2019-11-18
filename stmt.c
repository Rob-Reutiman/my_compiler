
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
			// tab print
			decl_print(s->decl, indent);
			break;
		case STMT_EXPR:
			// tab print
			expr_print(s->expr, stdout);
			printf(";\n");
			break;
		case STMT_IF_ELSE: 
			tab_print(indent);
			printf("if( ");
			expr_print(s->expr, stdout);
			printf(" )\n");
			tab_print(indent);
			printf("{");
			if(s->body->kind == STMT_DECL || s->body->kind == STMT_EXPR) {
				printf("\n");
				tab_print(indent+1);
			} else {
				printf("\n");
			}
			stmt_print(s->body, indent+1);
			tab_print(indent);
			printf("}");
			if(s->else_body) {
				printf(" else {");
				if(s->else_body->kind == STMT_DECL || s->else_body->kind == STMT_EXPR) {
					printf("\n");
					tab_print(indent+1);
				} else {
					printf("\n");
				}
				stmt_print(s->else_body, indent+1);
				tab_print(indent);
				printf("}\n");
			} else {
				printf("\n");
			}
			break;
		case STMT_FOR:
			tab_print(indent);
			printf("for( ");
			expr_print(s->init_expr, stdout);
			printf("; ");
			expr_print(s->expr, stdout);
			printf("; ");
			expr_print(s->next_expr, stdout);
			printf("; )\n");
			tab_print(indent);
			printf("{\n");
			stmt_print(s->body, indent+1);
			printf("\n");
			tab_print(indent);
			printf("}\n");
			break;
		case STMT_PRINT:
			tab_print(indent);
			printf("print ");
			expr_print(s->expr, stdout);
			printf(";");
			break;
		case STMT_RETURN:
			tab_print(indent);
			printf("return ");
			expr_print(s->expr, stdout);
			printf(";");
			break;
		case STMT_BLOCK:
			stmt_print(s->body, indent);
			if(s->next) {
				if(s->next->kind == STMT_EXPR || s->next->kind == STMT_DECL) {
		//			printf("\n");
					tab_print(indent);
				}
				stmt_print(s->next, indent);
			}
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

void stmt_resolve( struct stmt *s, struct hash_table *h) {

	if(!s) return;

	decl_resolve(s->decl, h);
	expr_resolve(s->init_expr, h);
	expr_resolve(s->expr, h);
	expr_resolve(s->next_expr, h);
	if(s->kind == STMT_BLOCK) scope_enter(&h);
	stmt_resolve(s->body, h);
	if(s->kind == STMT_BLOCK) scope_exit(&h);
	stmt_resolve(s->else_body, h);
	stmt_resolve(s->next, h);

}

void stmt_typecheck( struct stmt *s) {

	if(!s) return;

	struct type *t;
	switch(s->kind) {
		case STMT_DECL:
			decl_typecheck(s->decl);
			break;
		case STMT_EXPR:
			t = expr_typecheck(s->expr);
			type_delete(t);
			break;
		case STMT_IF_ELSE:
			t = expr_typecheck(s->expr);
			if(t->kind!=TYPE_BOOLEAN) {
				fprintf(stderr, "type error: if expression ");
				expr_print(s->expr, stderr);
				fprintf(stderr, " must be of type boolean\n");
				TYPE_ERROR = 0;
			}
			type_delete(t);
			stmt_typecheck(s->body);
			stmt_typecheck(s->else_body);
			break;
		case STMT_FOR:
			t = expr_typecheck(s->init_expr);
			if(t->kind!= TYPE_INTEGER) {
				fprintf(stderr, "type error: for init expression ");
				expr_print(s->init_expr, stderr);
				fprintf(stderr, " must be of type integer\n");
				TYPE_ERROR = 0;
			}
			t = expr_typecheck(s->expr);
			if(t->kind!=TYPE_BOOLEAN) {
				fprintf(stderr, "type error: for expression ");
				expr_print(s->expr, stderr);
				fprintf(stderr, " must be of type boolean\n");
				TYPE_ERROR = 0;
			}
			t = expr_typecheck(s->next_expr);
			if(t->kind!=TYPE_INTEGER) {
				fprintf(stderr, "type error: for next expression ");
				expr_print(s->next->expr, stderr);
				fprintf(stderr, " must be of type integer\n");
				TYPE_ERROR = 0;
			}
			type_delete(t);
			stmt_typecheck(s->body);
			break;
		case STMT_PRINT:
			t = expr_typecheck(s->expr);
			break;
		case STMT_RETURN:
			t = expr_typecheck(s->expr);
			break;
		case STMT_BLOCK:
			stmt_typecheck(s->body);
			break;
	}

	stmt_typecheck(s->next);

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

