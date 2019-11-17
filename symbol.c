
#include "symbol.h"
#include <stdlib.h>
#include <string.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ) {

	struct symbol* s = malloc(sizeof(struct symbol));

	s->kind = kind;
	s->type = type;
	if(name) {
		s->name = strdup(name);
	}
//	s->which = ;

	return s;

}

void symbol_delete(struct symbol * s ) {
	if(!s) return;
	type_delete(s->type);

	free(s);
}

