
#include "stmt.h"
#include <stdlib.h>
#include "library.c"

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
			// how to account for returning thing from void
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

void stmt_codegen(struct stmt *s, FILE* stream) {
	
	if(!s) return;

	int else_label;
	int top_label;
	int done_label;
	struct expr * e;

	struct type *t;
	switch(s->kind) {
		case STMT_DECL:
			decl_codegen(s->decl, stream);
			break;
		case STMT_EXPR:
			expr_codegen(s->expr, stream);
			scratch_free(s->expr->reg);
			break;
		case STMT_IF_ELSE:
			else_label = label_create();
			done_label = label_create();
			expr_codegen(s->expr, stream);
			fprintf(stream, "CMP $0, %s\n", scratch_name(s->expr->reg));
			scratch_free(s->expr->reg);
			fprintf(stream, "JE %s\n", label_name(else_label));
			stmt_codegen(s->body, stream);
			fprintf(stream, "JMP %s\n", label_name(done_label));
			fprintf(stream, "%s:\n", label_name(else_label));
			stmt_codegen(s->else_body, stream);
			fprintf(stream, "%s:\n",label_name(done_label));
			break;
		case STMT_FOR:
			expr_codegen(s->init_expr, stream);
			top_label = label_create();
			done_label = label_create();
			fprintf(stream, "%s:\n", label_name(top_label));
			expr_codegen(s->expr, stream);
			fprintf(stream, "CMP $0, %s\n", scratch_name(s->expr->reg));
			fprintf(stream, "JE %s\n", label_name(done_label));
			stmt_codegen(s->body, stream);
			expr_codegen(s->next_expr, stream);
			fprintf(stream, "JMP %s\n", label_name(top_label));
			fprintf(stream, "%s:\n", label_name(done_label));
			scratch_free(s->init_expr->reg);
			scratch_free(s->expr->reg);
			scratch_free(s->next_expr->reg);
			break;
		case STMT_PRINT:
			if(s->expr->kind == EXPR_ARGLIST) {
				e = s->expr->left;
			} else {
				e = s->expr;
			}
			while(e) {
				expr_codegen(e, stream);
				fprintf(stream, "MOVQ %s, %rdi\n", scratch_name(s->expr->reg));
				if(e->kind == EXPR_NAME) {
					switch(e->symbol->type->kind) {
						case TYPE_STRING: 
							fprintf(stream, "CALL print_string\n");
							break;
						case TYPE_INTEGER:
							fprintf(stream, "CALL print_integer\n");
							break;
						case TYPE_BOOLEAN: 
							fprintf(stream, "CALL print_boolean\n");
							break;
						case TYPE_CHARACTER: 
							fprintf(stream, "CALL print_character\n");
							break;
					}	
				} else {

				switch(e->kind) {
					case EXPR_STRING_LITERAL: 
						fprintf(stream, "CALL print_string\n");
						break;
					case EXPR_INTEGER_LITERAL:
						fprintf(stream, "CALL print_integer\n");
						break;
					case EXPR_BOOLEAN_LITERAL: 
						fprintf(stream, "CALL print_boolean\n");
						break;
					case EXPR_CHAR_LITERAL: 
						fprintf(stream, "CALL print_character\n");
						break;
					default:
						fprintf(stream, "CALL print_integer\n");
						break;
				}
				} 
				scratch_free(e->reg);
					if (s->expr->right == NULL || s->expr->left == NULL) {
						e=NULL; 
					} else if(s->expr->right->kind == EXPR_ARGLIST) {
						s->expr=s->expr->right;
						e=s->expr->left;
					} else {
						e=s->expr->right;
						s->expr->right = NULL;
					}
			}	
			break;
		case STMT_RETURN:
			expr_codegen(s->expr, stream);
			fprintf(stream, "MOVQ %s, %rax\n", scratch_name(s->expr->reg));
			scratch_free(s->expr->reg);
			break;
		case STMT_BLOCK:
			stmt_codegen(s->body, stream);
			break;
	}

	stmt_codegen(s->next, stream);

}
