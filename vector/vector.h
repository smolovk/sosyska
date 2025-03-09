#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
	char **data;
	size_t size;
	size_t capacity;
} StringVector;

#include "./vector.c"

void initVector(StringVector *vec, size_t capacity);
void pushBack(StringVector *vec, const char *str);
void freeVector(StringVector *vec);
char* popBack(StringVector *vec);

#endif
