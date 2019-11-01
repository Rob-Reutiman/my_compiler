
#include "symbol.h"

// Delete from here to

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type *type;
	char *name;
	int which;
};


// here. All of this is still here just for quick and easy reference

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ) {

	struct symbol* s = calloc((size_t)1, sizeof(symbol));

	s->kind = kind;
	s->type = type;
	if(name) {
		s->name = strdup(name);
	}
	s->which = ;

	return s;

}

void symbol_delete(struct symbol * s ) {
	if(!s) return;
	type_delete(s->type);

	if(s->name) {
		free(s->name);
	}

	free(s);
}
