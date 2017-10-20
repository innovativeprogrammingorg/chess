#ifndef STR_H
#define STR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "vector.h"

Vector explode(char * quan,char * subject);
Vector split(char quan,char * subject);
Vector ssplit(char* quan,char * subject);
char* str_reverse(char* str);
char* substring(char * subject,size_t index,int length);
char* substr(char * subject,int index);
uint_least8_t strcompare(char * str1, char * str2);
uint_least8_t strcompsub(char * str1,char * str2, size_t index, size_t length);
size_t strlength(char * str);
char* concat(char * s1, char * s2, uint8_t mem);
char* concat_all(int args,...);


int64_t strpos(char * haystack, char * needle);
char* substring_f(char * subject,size_t index,size_t length);
char* substr_f(char * subject,size_t index);
char lastChar(char * str);
char* strappend(char s,char * str);
char* stradd(char * str,char s);
char* itoa(uint32_t num);
char* ltoa(uint64_t num);
char getLast(char * str);
int indexOfChar(char * haystack, char needle);
char * trim(char* str);
char* str_replace(char* search,char* replace,char* subject);

char* ktrim(char* str);

#define TRUE 1
#define FALSE 0
#define FIRST 1
#define SECOND 2

#endif