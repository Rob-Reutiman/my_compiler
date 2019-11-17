#ifndef SCOPE_H
#define SCOPE_H

#include "type.h"
#include "hash_table.h" 

void scope_enter(struct hash_table*);
void scope_exit(struct hash_table*);
int scope_level(struct hash_table*);
void scope_bind(struct hash_table*, char* name, struct symbol *sym);
struct symbol* scope_lookup(struct hash_table*, char* name);
struct symbol* scope_lookup_current(struct hash_table*, char* name);

#endif
