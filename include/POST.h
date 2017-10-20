#ifndef _POST_H_
#define _POST_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "io.h"
#include "map.h"

char* process_through_PHP(Map _POST,char* dir);
char* parse_PHP_args(Map args);


#endif