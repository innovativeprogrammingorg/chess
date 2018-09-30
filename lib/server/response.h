#ifndef _SERVER_RESPONSE_H_
#define _SERVER_RESPONSE_H_


#include <string>
#include <unistd.h>
#include "str.h"
#include "server/HTTP.h"


using namespace std;

struct response{
	string* header;
	char* body;
	size_t data_size;
};

typedef struct response* Response;

#include "server/io.h"
#include "server/header.h"

Response new_response(string* header, char* body,size_t data_size);
Response GET_response(HTTP_Request* r);


#endif