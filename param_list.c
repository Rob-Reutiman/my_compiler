
#include "decl.h"
#include "type.h"
#include "symbol.h"
#include "param_list.h"
#include <stdlib.h>

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ) {

	struct param_list *p = malloc(sizeof(*p));

	p->name = name;
	p->type = type;
	p->next = next;

	return p;

}

void param_list_print( struct param_list *a ) {

	printf("%s: ", a->name);
   	type_print(a->type, stdout);
	if(a->next) {
		printf(", ");
		param_list_print(a->next);
	}

}

void param_list_resolve( struct param_list *a, struct hash_table *h) {

	if(!a) return;

	struct decl *d = decl_create(a->name, a->type, NULL, NULL, NULL); 

	symbol_t kind = SYMBOL_PARAM;
	d->symbol = symbol_create(kind, d->type, d->name);
	scope_bind(h, d->name, d->symbol);

	param_list_resolve(a->next, h);

}

struct param_list *param_list_copy(struct param_list *a) {

	if(!a) return NULL;

	struct param_list *p = param_list_create(a->name, type_copy(a->type), param_list_copy(a->next));
	p->symbol = symbol_copy(a->symbol);

	return p;
}

int param_list_equals(struct param_list *a, struct param_list *b) {
	if(!strcmp(a->name, b->name) && type_equals(a->type, b->type) && param_list_equals(a->next, b->next) ) {
		return 0;
	} else {
		return 1;
	}
}

void param_list_delete(struct param_list * a ) {
	if(!a) return;

	if(a->name) {
		free(a->name);
	}

	type_delete(a->type);
	symbol_delete(a->symbol);
	param_list_delete(a->next);

	free(a);
}
