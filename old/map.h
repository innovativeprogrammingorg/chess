#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct map* Map;

struct map{
	int64_t key;
	int64_t data;
	Map next;
	Map prev;
};

Map createMap();
void map_update(Map* v, int64_t key,int64_t data);
int64_t map_lookup(Map m,int64_t key);


#endif