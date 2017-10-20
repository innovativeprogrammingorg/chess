#ifndef json_H
#define json_H

#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include "str.h"

typedef struct json* JSON;
struct json{
	Vector keys;
	Vector values;
	JSON * data;
	char * type;
	char * name;
	char * url;
};
JSON ati(JSON j,size_t i);
JSON atc(JSON j,char * key);
char * valueAtc(JSON j, char * key);
char * valueAti(JSON j,size_t index);
JSON createJSON(char * t);
void adds(JSON j, char * element);
void addj(JSON j,JSON element);
void addss(JSON j,char * key,char * element);
void addsj(JSON j, char * key,JSON element);
char* keyAt(JSON j, size_t index);
size_t size(JSON j);
void addContents(JSON j, Vector input);
char* jsonToString(JSON data);


#endif
