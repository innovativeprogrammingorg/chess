#ifndef _COOKIES_H_
#define _COOKIES_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>
#include "io.h"
#include "map.h"
#include "server.h"

typedef struct cookie * Cookie;

struct cookie{
	char* value;
	time_t created;
	time_t expires;
};

Cookie new_cookie(char* value,time_t created,time_t exp);
uint8_t store_cookie(char* ip, char* key, Cookie c);
Map load_cookies(char* ip);
uint8_t parse_cookies(char* ip,Map header);


#ifndef COOKIES_DIR
#define COOKIES_DIR "./tmp/cookies/"
#endif

#ifndef COOKIES_EXP
#define COOKIES_EXP 604800 /* 1 week*/
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif