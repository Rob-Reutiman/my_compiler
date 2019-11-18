#include "scope.h"
#include "hash_table.h"
#include "param_list.h"
#include "symbol.h"
#include <stdlib.h>
#include <string.h>

void scope_enter(struct hash_table** head) {
	if(*head == NULL) {
		*head = hash_table_create(0, 0);
		return;
	}

	struct hash_table *temp = *head;
	*head = hash_table_create(0,0);
	(*head)->next = temp;
	
}

void scope_exit(struct hash_table** head) {
	if(!(*head)) {
		return;
	}

	struct hash_table *temp = *head;
	*head = (*head)->next;
	hash_table_delete(temp);

}

int scope_level(struct hash_table* head) {

	int level = 0;
	struct hash_table* h = head;

	while(h != NULL) { 
		level++;
		h = h->next;
	}

	return level;
	
}

void scope_bind(struct hash_table* head, char* name, struct symbol *sym) {

	if(!head->next) {
		sym->which = hash_table_size(head);
	} else {
		char* key; struct symbol *val;
		int size = 0;
		struct hash_table* curr = head;
		while(curr->next) {
			hash_table_firstkey(curr);
			while(hash_table_nextkey(curr, &key, (void*)&val)) {
				if(val->kind == sym->kind) size++;
			}

			curr = curr->next;
		}
		sym->which = size;	
	}

	switch(sym->kind) {
		case SYMBOL_LOCAL:
			printf("%s resolves to local %d\n", sym->name, sym->which);
			break;
		case SYMBOL_GLOBAL:
			printf("%s resolves to global %s\n", sym->name, sym->name);
			break;
		case SYMBOL_PARAM:
			printf("%s resolves to param %d\n", sym->name, sym->which);
			break;
	}
	if(hash_table_insert(head, name, sym) != 1) {
		fprintf(stderr, "Scope Bind Failure\n");	
	}
}

struct symbol *scope_lookup(struct hash_table* head, const char* name) {
	struct hash_table *h = head;

	while(h != NULL) { 
		struct symbol *s = scope_lookup_current(h, name);
		if(s) {
			return s;
		}
		h=h->next;
	}

	return NULL;
}

struct symbol *scope_lookup_current(struct hash_table* head, const char* name) {

	struct symbol *s = hash_table_lookup(head, name);
	return s;
}
