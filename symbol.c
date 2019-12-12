
#include "symbol.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ) {

	struct symbol* s = malloc(sizeof(struct symbol));

	s->kind = kind;
	s->type = type;
	s->name = strdup(name);

	return s;

}

struct symbol *symbol_copy(struct symbol *s) {
	return symbol_create(s->kind, type_copy(s->type), strdup(s->name));
}

void symbol_delete(struct symbol * s ) {
	if(!s) return;
	type_delete(s->type);

	free(s);
}

const char* symbol_codegen( struct symbol *s) {

	if(!s) return NULL;

	char position[50];
	char* str;

	switch(s->kind) {
		case SYMBOL_GLOBAL:
			return s->name;
		case SYMBOL_PARAM:
			sprintf(position, "-%d(%rbp)", s->which);
			str = strdup(position);
			return str;
		case SYMBOL_LOCAL:
			sprintf(position, "-%d(%rbp)", s->which);
			str = strdup(position);
			return str;
		default:
			return NULL;
	}
	
}
