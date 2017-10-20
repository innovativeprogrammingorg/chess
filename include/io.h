#ifndef _IO_H_
#define _IO_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "str.h"
#include "base64.h"

#include "response.h"
/**
 * Read from file descriptor
 * @param  fd   The file descriptor to be read from
 * @param  data The destination for the data
 * @return      TRUE/FALSE (Failure/Success)
 */
uint64_t fread_file(FILE * fd,char** data);
uint64_t sread_file(char* directory,char** data);
uint64_t prepare_media(char* directory,char** data);
uint64_t read_file(int fd,char** data);
char * get_content_type(char* directory);
void srespond(int fd, char * data);
void respond(int fd, Response r);
void force_print(char* in,size_t length);

#ifndef FILE_BUFFER_SIZE
#define FILE_BUFFER_SIZE 1000000
#endif

#ifndef WEB_ROOT
#define WEB_ROOT "./www"
#endif

#endif