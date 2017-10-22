#ifndef _CGI_H_
#define _CGI_H_

#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdint.h>
#include "../str.h"
#include "../response.h"
#include "../data_types.h"
#include "../client.h"

void start_PHP_CGI(int port);
uint64_t* run_CGI(char* message,char* script,char ** data);
Response CGI_response(Map m);


#endif