#ifndef STR_H
#define STR_H

#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <stdarg.h>
#include <vector>

using namespace std;
vector<string>* c_explode(char quan,string subject);
vector<string>* explode(string quan,string subject);
vector<char*> split(char quan,char * subject);
vector<char*> ssplit(char* quan,char * subject);
char* str_reverse(char* str);
char* substring(char * subject,size_t index,int length);
char* substr(char * subject,int index);
uint_least8_t strcompare(char * str1, char * str2);
uint_least8_t strcompsub(char * str1,char * str2, size_t index, size_t length);
size_t strlength(char * str);
char* concat(char * s1, char * s2, uint8_t mem);
char* concat_all(int args,...);
string itoa(int i);
int64_t strpos(char * haystack, char * needle);
char* substring_f(char * subject,size_t index,size_t length);
char* substr_f(char * subject,size_t index);
char lastChar(char * str);
char* strappend(char s,char * str);
char* stradd(char * str,char s);
char getLast(string str);
int indexOfChar(string haystack, char needle);
char * trim(char* str);
char* str_replace(char* search,char* replace,char* subject);

char* trim_f(char* str);


char* to_cstr(string s);
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef FIRST
#define FIRST 1
#endif

#ifndef SECOND
#define SECOND 2
#endif

#endif