#ifndef _CONCURRENCY_H_
#define _CONCURRENCY_H_

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include "data_types.h"
#include "request.h"

#include "vector.h"



WThread new_WThread(Request r);
void create_new_thread(Client c, char* data);
void * handle_request(void * wt);
#endif