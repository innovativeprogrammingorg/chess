#ifndef _REQUEST_H_
#define _REQUEST_H_


#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include "io.h"
#include "data_types.h"

using namespace std;
Request new_request(void * client, char* data);
bool is_PHP_request(string message, string* directory);

#ifndef TRUE 
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 1
#endif

#endif