#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include "str.h"
#include "map.h"

struct response{
	char* header;
	char* body;
	size_t data_size;
};

typedef struct response* Response;

#include "io.h"
#include "header.h"
#include "POST.h"

Response new_response(char* header, char* body,size_t data_size);
Response e404_response(Map header);
Response e400_response(Map header);
Response GET_response(Map header);
char* get_requested_directory(Map m);
Response POST_response(Map header);


#endif