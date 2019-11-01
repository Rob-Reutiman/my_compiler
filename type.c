#include "type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params ) {

	struct type *t = calloc((size_t)1, sizeof(type));

	type->kind = kind;
	type->params = params;
	if(subtype) {
		type->subtype = subtype;
	}

}

void type_print( struct type *t ) {

	switch(t->kind) {

		case TYPE_VOID:
		   	printf("void");	
			break;

		case TYPE_BOOLEAN: 
			printf("boolean");
			break;
	
		case TYPE_CHARACTER: 
			if(t->params != NULL) {
				printf("function 
			}
			break;
	
		case TYPE_INTEGER: 
			if(t->params != NULL) {
				printf("function 
			}
			break;

		case TYPE_STRING: 
			if(t->params != NULL) {
				printf("function 
			}
			break;

		case TYPE_ARRAY: 
			printf("array %s %s", expr_print(), type_print(t->kind));
			break;

		case TYPE_FUNCTION: 
			if(t->params != NULL) {
				printf("function 
			}
			break;

		default:
			printf("type error");
			break;
	}

}

void type_delete(struct type * t ) {
	if(!t) return;
	param_list_delete(t->params);
	type_delete(t->subtype);

	free(t);
}

