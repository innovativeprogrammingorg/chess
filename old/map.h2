#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"


typedef struct meta * Meta;

struct meta{
	char* key;
	void* value;
	Meta next;
	Meta prev;
	uint8_t type;
};
/**
 * Map is used to connect keys to values, keys must be char*, values are of type void*
 */
typedef struct map * Map;

struct map{
	Meta head;
};

uint8_t map_has_key(Map m, char* key);
void map_add(Map* m, char* key, void* value,uint8_t type);
void map_update(Map *m, char* key, void* value, uint8_t type);

void map_remove(Map* m, char* key);
void* map_value_at(Map m, char* key);
char** map_get_keys(Map m);
size_t map_size(Map m);
void map_clean(Map m);
void print_map_contents(Map m);

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef STRING_TYPE
#define STRING_TYPE 0
#endif

#ifndef MAP_TYPE
#define MAP_TYPE 1
#endif

#ifndef ARRAY_TYPE
#define ARRAY_TYPE 2
#endif

#ifndef VECTOR_TYPE
#define VECTOR_TYPE 3
#endif

#ifndef INTEGER_TYPE
#define INTEGER_TYPE 4
#endif

#ifndef COOKIE_TYPE
#define COOKIE_TYPE 5
#endif

#ifndef ULONGINT_TYPE
#define ULONGINT_TYPE 6
#endif

#endif