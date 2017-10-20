#ifndef _BASE64_H_
#define _BASE64_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>

char* convertToBinary(char* input);
char convertValue(uint8_t a);
char* convertToBase64(char * input);

#endif