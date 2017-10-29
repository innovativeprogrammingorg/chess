#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <netdb.h> /* struct hostent, gethostbyname */
#include "concurrency.h"
#include "str.h"
#include <signal.h>
#include <time.h>
/*#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>*/
#include <resolv.h>
#include "client.h"
#include <vector>

#ifndef TRUE
#define TRUE   1
#endif

#ifndef FALSE
#define FALSE  0
#endif

#define BUFFER_SIZE 200000 

void segfault_catch(int signum);
void kill_all(int signum);

#endif