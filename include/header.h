#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdio.h>
#include <time.h>
#include <string>
#include <iostream>
#include "str.h"
#include "response.h"
#include "request.h"
#include "HTTP.h"

using namespace std;


string get_status_line(int status);
/**
 * Checks if the header received is valid
 * @param  m [description]
 * @return   1 if the header appears to be valid, otherwise returns an HTTP error code
 */
uint16_t check_valid_params(HTTP_Request* r);
string get_date_line();
uint8_t get_request_type(HTTP_Request* r);
string get_server_line();
string get_content_length_line(uint64_t content_length);
string get_connection_line(HTTP_Request* r);
string get_content_type_line(string content);
Response build_response(HTTP_Request* r);
string get_vary_line();
string get_content_encoding_line(string encode);
string get_accept_ranges_line();

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