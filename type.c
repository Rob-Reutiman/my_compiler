#include "type.h"
#include <stdlib.h>

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params ) {

	struct type *t = malloc(sizeof(*t));

	t->kind = kind;
	t->params = params;
	t->subtype = subtype;

	return t;
}

void type_print( struct type *t ) {

	if(!t) return;

	switch(t->kind) {

		case TYPE_VOID:
		   	printf("void");	
			break;

		case TYPE_BOOLEAN: 
			printf("boolean");
			break;
	
		case TYPE_CHARACTER: 
			printf("char");
			break;
	
		case TYPE_INTEGER: 
			printf("integer");
			break;

		case TYPE_STRING: 
			printf("string");
			break;

		case TYPE_ARRAY: 
			printf("array ");
			type_print(t->subtype);
			break;

		case TYPE_AUTO:
			printf("auto");
			break;

		case TYPE_FUNCTION: 
			printf("function ");
		   	type_print(t->subtype);
			printf(" (");
			if(t->params) {
				printf(" ");
				param_list_print(t->params);
				printf(" ");
			}
			printf(")");
			break;
		default:
			break;
	} 

}

void type_delete(struct type * t ) {
	if(!t) return;
	param_list_delete(t->params);
	type_delete(t->subtype);

	free(t);
}

