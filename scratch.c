#include "scratch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int scratch_alloc() {

	int i;

	for(i = 0; i < 5; i++) {
		if(SCRATCH_TABLE[i] == 0) {
			SCRATCH_TABLE[i] = 1;
			return i;
		}
	}

	fprintf(stderr, "Cannot allocate reg\n");
	exit(EXIT_FAILURE);
}

void scratch_free( int r ) {

	if(r < 0 || r > 5) return;

	SCRATCH_TABLE[r] = 0;

	return;
}

const char* scratch_name(int r ) {

	switch(r) {
		case 0:
			return "%rbx";
		case 1:
			return "%r10";
		case 2:
			return "%r11";
		case 3:
			return "%r12";
		case 4:
			return "%r13";
		case 5:
			return "%r14";
		case 6:
			return "%r15";
		default:
			return NULL;
	}

}

int label_create() {
	COUNTER++;
	return COUNTER;
}

const char* label_name( int label ) {
	char s[3]; 
	sprintf(s, ".L%d", label);
	char* name = strdup(s);
	return name;
}
