#ifndef _CONCURRENCY_H_
#define _CONCURRENCY_H_

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include "data_types.h"
#include "client.h"
#include "WebSocket/control.h"



using namespace std;


Request new_request(void * client, char* data);
WThread new_WThread(Request r);
void handshake(Client* c, char* data,size_t size);
void* handle_handshake(void* wt);
void data_frame(Client* c, char* data,size_t size);
void* handle_data_frame(void* wt);
#endif