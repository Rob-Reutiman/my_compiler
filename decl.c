
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
	type_print(d->type);
	printf(" = ");
	expr_print(d->value);
	
	if(d->code) {
		printf("\n");
	}
	if(d->type->kind == TYPE_FUNCTION) {
		printf("{\n");
		stmt_print(d->code, indent+1);
		printf("\n}\n");
	} else {
		stmt_print(d->code, indent+1);
	}

	
		// symbol print
	if(d->next) {
		printf("\n");
	}
	decl_print(d->next, indent);

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
