
#include "param_list.h"

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ) {

	struct param_list *p = calloc((size_t)1, sizeof(param_list));

	p->name = name;
	p->type = type;
	p->next = next;

}

void param_list_print( struct param_list *a ) {

	printf("%s : ", a->name);
   	type_print(a->type);
	if(a->next) {
		printf(", ");
		param_list_print(a->next);
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
