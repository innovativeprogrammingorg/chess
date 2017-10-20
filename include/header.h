#ifndef _HEADER_H_
#define _HEADER_H_

#include "str.h"
#include "vector.h"
#include "map.h"
#include "response.h"
#include "request.h"
#include "./CGI/CGI.h"

#include <stdio.h>
#include <time.h>


Map parse_HTTP_header(char * header);
Map parse_HTTP_body(char * body);
Map parse_HTTP_message(char * message);
char * get_status_line(int status);
/**
 * Checks if the header received is valid
 * @param  m [description]
 * @return   1 if the header appears to be valid, otherwise returns an HTTP error code
 */
uint16_t check_valid_params(Map m);
char * get_date_line();
uint8_t get_request_type(Map m);
char * get_server_line();
char* get_content_length_line(uint64_t content_length);
char* get_connection_line(Map header);
char* get_content_type_line(char* content);
Response build_response(Map m);
char* get_vary_line();
char* get_content_encoding_line(char * encode);
char* get_accept_ranges_line();

/**
 * REQUEST TYPE CONSTANTS
 * For now, do not over write any of the same name if they exist
 */

#ifndef GET
#define GET 1
#endif

#ifndef POST
#define POST 2
#endif

#ifndef PUT
#define PUT 3
#endif

#ifndef DELETE
#define DELETE 4
#endif

#ifndef HEAD
#define HEAD 5
#endif

#ifndef OPTIONS
#define OPTIONS 6
#endif

#ifndef CONNECT
#define CONNECT 7
#endif

#ifndef UNKNOWN
#define UNKNOWN 8
#endif

#endif