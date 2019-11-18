#include "type.h"
#include "param_list.h"
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

int type_equals( struct type *a, struct type *b ) {
	if( a->kind == b->kind ) {
		if( a->kind == TYPE_CHARACTER || a->kind == TYPE_BOOLEAN || a->kind == TYPE_INTEGER || a->kind == TYPE_STRING || a->kind == TYPE_AUTO  ) {
			return 0;
		} else if ( a->kind == TYPE_ARRAY ) {
			if(type_equals(a->subtype, b->subtype)) return 0;
		} else if ( a->kind == TYPE_FUNCTION ) {
			if(type_equals(a->subtype, b->subtype) && param_list_equals(a->params, b->params)) return 0; 
		}
	} else {
		return 1;
	} 
}

struct type * type_copy( struct type *t ) {
	return type_create(t->kind, type_copy(t->subtype), param_list_copy(t->params) );
}

void type_delete(struct type * t ) {
	if(!t) return;
	param_list_delete(t->params);
	type_delete(t->subtype);

	free(t);
}

