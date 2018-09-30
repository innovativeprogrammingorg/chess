#ifndef _IO_H_
#define _IO_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "str.h"
#include "server/base64.h"
#include "server/response.h"
/**
 * Read from file descriptor
 * @param  fd   The file descriptor to be read from
 * @param  data The destination for the data
 * @return      TRUE/FALSE (Failure/Success)
 */

namespace io{
	uint64_t read_file(FILE * fd,char** data);
	uint64_t read_file(char* directory,char** data);
	uint64_t read_file(int fd,char** data);

	string get_content_type(string directory);

	void respond(int fd, char * data);
	void respond(int fd, const string& data);
	void respond(int fd, Response r);
	void force_print(char* in,size_t length);
	void append_to_file(const string& dir, const string& filename, const string& data);
	void write_to_file(const string& dir, const string& filename, const string& data);
	string read_from_file(const string& dir,const string& filename = "");
	uint64_t read_from_file(const string& dir,const string& filename, string* data);
	char* force_concat(char* s1,size_t l1,char*s2,size_t l2,uint8_t mem);
	void create_tmp_file(char* directory);
}


#ifndef FILE_BUFFER_SIZE
#define FILE_BUFFER_SIZE 1000000
#endif

#ifndef WEB_ROOT
#define WEB_ROOT "./www"
#endif

#endif