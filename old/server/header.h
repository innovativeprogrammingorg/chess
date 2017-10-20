#ifndef HEADER_H
#define HEADER_H

#include "../chess.h"
#include "b64.h"

void server_handshake(char* request,int fd);
int request_type(char* header);
JSON parseRequest(Vector header);
char* generateResponse(int request, JSON header,Vector fields);
char* convToBase64(uint8_t* hex,size_t ilength);
char* base64_encode(char* str);
char* uintToHex(uint8_t * in,size_t size);
char* sec_webkey(char* str);
int hexdec(char* in);

#define GET 1
#define POST 2
#define UPDATE 4
#define DELETE 8

#endif