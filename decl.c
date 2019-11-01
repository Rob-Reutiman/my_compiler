
#include "decl.h"

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {

	struct decl *d = calloc((size_t)1, sizeof(decl));

	if(name) {
		decl->name = strdup(name);
	}
	decl->type = type;

	if(value) {
		decl->value = value;
		decl->code = code;
		decl->symbol = symbol_create(kind, type, name);
		decl->next = next;
	}

	return d;
}

void decl_print( struct decl *d, int indent ) {

	while(indent > 0) {
		printf("\t");
		ident--;
	}

	printf("%s : ", d->name);
	type_print(d->type);

	if(d->value) {
		expr_print(d->value);
		stmt_print(d->code);
		// symbol print, next decl_print ???

	} else {
		printf(";");
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
