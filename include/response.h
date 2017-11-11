#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include "str.h"
#include "HTTP.h"
#include <string>
#include <string.h>
using namespace std;

struct response{
	string* header;
	char* body;
	size_t data_size;
};

typedef struct response* Response;

#include "io.h"
#include "header.h"

Response new_response(string* header, char* body,size_t data_size);
Response GET_response(HTTP_Request* r);


#endif