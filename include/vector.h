#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vect* Vector;
struct vect{
	Vector next;
	Vector prev;
	void* data;
};
size_t vector_length(Vector v);
void* vector_get(Vector v,size_t index);
/**
 * Frees the vector structs and data
 * @param v [description]
 */
void vector_clean(Vector v);

/**
 * Frees just the vector structs
 * @param v [description]
 */
void vector_free(Vector v);
void vector_push(Vector* v,void* data);
Vector vector_merge(Vector* v1, Vector* v2);
void vector_pop(Vector* v, size_t index);
void* vector_get_f(Vector v,size_t index);
void vector_print_i(Vector v1);
void ** vector_to_array(Vector v);

#endif