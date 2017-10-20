#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include "cookies.h"
#include "vector.h"
#include "map.h"
#include "str.h"
#include "data_types.h"


Client new_client(int fd,char* ip,int port);

Client find_client_by_ip(Vector clients,char* ip);


#ifndef CLIENT_TIMEOUT
#define CLIENT_TIMEOUT 600
#endif


#endif