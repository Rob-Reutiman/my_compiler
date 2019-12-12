
#include "decl.h"
#include <stdlib.h> 
#include <string.h>

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {

	struct decl *d = malloc(sizeof(struct decl));

	d->name = strdup(name);
	d->type = type;

	d->value = value;
	d->code = code;
	d->next = next;

	return d;
}

void decl_print( struct decl *d, int indent ) {

	if(!d) return;

	int temp = indent;

	while(temp > 0) {
		printf("\t");
		temp--;
	}

	printf("%s: ", d->name);
	type_print(d->type, stdout);
	if(d->type->kind == TYPE_ARRAY && d->value) {
		fprintf(stdout, " = {");
	}
	expr_print(d->value, stdout);
	if(d->type->kind == TYPE_ARRAY && d->value) {
		fprintf(stdout, "}");
	}
	
	if(d->code) {	
		printf(" = ");
		if(d->type->kind == TYPE_FUNCTION) {
			printf("\n{\n");
			stmt_print(d->code, indent+1);
			printf("\n}\n");
		} else {
			stmt_print(d->code, indent+1);
		}
	} else {
		printf(";\n");
	}

	if(d->next) {
		printf("\n");
		decl_print(d->next, indent);
	}

}

void decl_resolve(struct decl* d, struct hash_table *h) {
	if(!d) return;

	symbol_t kind = scope_level(h) > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
	d->symbol = symbol_create(kind, d->type, d->name);
	expr_resolve(d->value, h);
	scope_bind(h, d->name, d->symbol);
	if(d->code) {
		scope_enter(&h);
		param_list_resolve(d->type->params, h);
		stmt_resolve(d->code, h);
		scope_exit(&h);
	}
	decl_resolve(d->next, h);
}

void decl_typecheck(struct decl *d) {
	if(!d) return;

	if(d->value) {
		struct type *t;
		t = expr_typecheck(d->value);
		if(d->symbol->type->kind == TYPE_AUTO) {

			d->symbol->type->kind = t->kind;
			d->symbol->type = t;
		}
		if(!type_equals(t, d->symbol->type)) {
			fprintf(stderr, "type error: cannot declare a ");
			type_print(t, stderr);
			fprintf(stderr, " (");
			expr_print(d->value, stderr);
			fprintf(stderr, ") equal to a ");
			type_print(d->symbol->type, stderr);
			fprintf(stderr, "\n");
			TYPE_ERROR = 0;
		}
	}

	if(d->code) {
		stmt_typecheck(d->code);
	}

}

void decl_codegen(struct decl *d, FILE* stream) {
	if(!d) return;

	switch(d->symbol->kind) {
		case SYMBOL_GLOBAL: 
			if(d->type->kind == TYPE_INTEGER || d->type->kind == TYPE_BOOLEAN || d->type->kind == TYPE_CHARACTER) {
				fprintf(stream, ".data\n%s: .quad %d", d->name, d->code->expr->literal_value);
			} else if (d->type->kind == TYPE_FUNCTION) {
				fprintf(stream, ".global %s\n", d->name);
				fprintf(stream, "%s:\n", d->name);
				fprintf(stream, "PUSHQ %rbp\n");
				fprintf(stream, "MOVQ %rsp, %rbp\n");

				param_codegen(d->symbol->type->params, stream);

				fprintf(stream, "PUSHQ %rbx\n");
				fprintf(stream, "PUSHQ %r12\n");
				fprintf(stream, "PUSHQ %r13\n");
				fprintf(stream, "PUSHQ %r14\n");
				fprintf(stream, "PUSHQ %r15\n");
				
				stmt_codegen(d->code, stream);

				fprintf(stream, "POPQ %r15\n");
				fprintf(stream, "POPQ %r14\n");
				fprintf(stream, "POPQ %r13\n");
				fprintf(stream, "POPQ %r12\n");
				fprintf(stream, "POPQ %rbx\n");

				fprintf(stream, "MOVQ %rbp, %rsp\n");
				fprintf(stream, "POPQ %rbp\n");
				fprintf(stream, "RET\n");
			} else {
				fprintf(stream, ".data\n%s: .", d->name);
				type_print(d->type, stream);
				fprintf(stream, " ");
				expr_print(d->code->expr, stream);
			}
			fprintf(stream, "\n");
			break;
		case SYMBOL_LOCAL:
			if(d->type->kind == TYPE_INTEGER || d->type->kind == TYPE_BOOLEAN || d->type->kind == TYPE_CHARACTER) {
				fprintf(stream, ".data\n%s: .quad %d\n", d->name, d->code->expr->literal_value);
			} else {
				fprintf(stream, "%s: .", d->name);
				type_print(d->type, stream);
				fprintf(stream, " ");
				expr_print(d->code->expr, stream);
			}
			break;

	}

}

void decl_delete(struct decl * d ) {
	if(!d) return;

	if(d->name) {
		free(d->name);
	}

	type_delete(d->type);
	expr_delete(d->value);
	stmt_delete(d->code);
	symbol_delete(d->symbol);
	decl_delete(d->next);

	free(d);
}
