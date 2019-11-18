
#include "decl.h"
#include <stdlib.h> 
#include <string.h>

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {

	struct decl *d = malloc(sizeof(struct decl));

	d->name = strdup(name);
	d->type = type;

	d->value = value;
	d->code = code;
	//	d->symbol = symbol_create(kind, type, name);
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
	expr_print(d->value, stdout);
	
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

	
		// symbol print
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

	expr_typecheck(d->value);
	stmt_typecheck(d->code);
	decl_typecheck(d->next);

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
