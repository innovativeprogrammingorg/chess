#ifndef _REQUEST_H_
#define _REQUEST_H_


#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include "client.h"
#include "data_types.h"

Request new_request(Client c, char* data);
uint8_t is_PHP_request(char * message, char** directory);

#ifndef TRUE 
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 1
#endif

#endif