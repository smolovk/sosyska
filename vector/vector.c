#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initVector(StringVector *vec, size_t capacity) {
	vec->data = malloc(capacity * sizeof(char *));
	vec->size = 0;
	vec->capacity = capacity;
}

void pushBack(StringVector *vec, const char *str) {
	if (vec->size == vec->capacity) {
		vec->capacity *= 2;
		vec->data = realloc(vec->data, vec->capacity * sizeof(char *));
	}
	vec->data[vec->size] = strdup(str);
	vec->size++;
}

void freeVector(StringVector *vec) {
	for (size_t i = 0; i < vec->size; i++) {
		free(vec->data[i]);
	}
	free(vec->data);
}

char* popBack(StringVector *vec) {
	char* ret;
	if (vec->size > 0) {
		char **data = (char **)vec->data;
		ret = strdup(data[vec->size-1]);
		free(data[vec->size - 1]);
		vec->size--;
		// TODO: we are allocating the ret var, and we need to free it when callling the func, try to find a better way
		return ret;
	}
	return NULL;
}

