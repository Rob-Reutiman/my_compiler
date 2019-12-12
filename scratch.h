#ifndef SCRATCH_H
#define SCRATCH_H

extern int COUNTER;
extern int SCRATCH_TABLE[6];

int scratch_alloc();
void scratch_free( int r );
const char* scratch_name( int r );

int label_create();
const char* label_name( int label );

#endif
