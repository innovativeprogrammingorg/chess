#ifndef _BASE64_H_
#define _BASE64_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <ctype.h>

char* convertToBinary(char* input);
char convertValue(uint8_t a);
uint8_t base64_value(char a);
char* b64_encode(uint8_t* input);
char* convertToBase64(char * input);
uint8_t* b64_decode(const char* src, size_t len, size_t* decsize = NULL);

#endif