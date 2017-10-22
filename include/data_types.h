#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_
#include <stdint.h>
#include <time.h>
#include <pthread.h>
#include "client.h"

typedef struct request* Request;

struct request{
	Client* client;
	char* data;
};

typedef struct thread_wrapper * WThread; 

struct thread_wrapper{
	Request req;
	pthread_t* thread;
};


#endif