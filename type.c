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

void type_print( struct type *t, FILE* stream ) {

	if(!t) return;

	switch(t->kind) {

		case TYPE_VOID:
		   	fprintf(stream, "void");	
			break;

		case TYPE_BOOLEAN: 
			fprintf(stream, "boolean");
			break;
	
		case TYPE_CHARACTER: 
			fprintf(stream, "char");
			break;
	
		case TYPE_INTEGER: 
			fprintf(stream, "integer");
			break;

		case TYPE_STRING: 
			fprintf(stream, "string");
			break;

		case TYPE_ARRAY: 
			fprintf(stream, "array ");
			type_print(t->subtype, stream);
			break;

		case TYPE_AUTO:
			fprintf(stream, "auto");
			break;

		case TYPE_FUNCTION: 
			fprintf(stream, "function ");
		   	type_print(t->subtype, stream);
			fprintf(stream, " (");
			if(t->params) {
				fprintf(stream, " ");
				param_list_print(t->params);
				fprintf(stream, " ");
			}
			fprintf(stream, ")");
			break;
		default:
			break;
	} 

}

int type_equals( struct type *a, struct type *b ) {
	if( a->kind == b->kind ) {
		if( a->kind == TYPE_CHARACTER || a->kind == TYPE_BOOLEAN || a->kind == TYPE_INTEGER || a->kind == TYPE_STRING || a->kind == TYPE_AUTO  ) {
			return 1;
		} else if ( a->kind == TYPE_ARRAY ) {
			if(type_equals(a->subtype, b->subtype)) return 1;
		} else if ( a->kind == TYPE_FUNCTION ) {
			if(type_equals(a->subtype, b->subtype) && param_list_equals(a->params, b->params)) return 1; 
		}
	} else {
		return 0;
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

