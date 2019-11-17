#include "scope.h"
#include <stdlib.h>
#include <string.h>

void scope_enter(struct hash_table* head) {
	if(!head) {
		hash_table_create(0, 0);
		return;
	}

	struct hash_table *temp = head;
	head = hash_table_create(0,0);
	head->next = temp;
	
}

void scope_exit(struct hash_table* head) {
	if(!head) {
		return;
	}

	struct hash_table *temp = head;
	head = head->next;
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
	
	if(hash_table_insert(head, name, sym) != 1) {
		fprintf(stderr, "Scope Bind Failure\n");	
	}
}

struct symbol *scope_lookup(struct hash_table* head, char* name) {
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

struct symbol *scope_lookup_current(struct hash_table* head, char* name) {

	struct symbol *s = hash_table_lookup(head, name);
	return s;
}
