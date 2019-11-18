#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"
#include "hash_table.h"

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_AUTO,
	TYPE_FUNCTION,
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
};

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params );
void type_print( struct type *t );
void type_resolve( struct type *t, struct hash_table *h);
void type_delete( struct type *t);

#endif
