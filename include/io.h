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
#include "base64.h"
#include "response.h"
/**
 * Read from file descriptor
 * @param  fd   The file descriptor to be read from
 * @param  data The destination for the data
 * @return      TRUE/FALSE (Failure/Success)
 */

class IO{
	public:
		static uint64_t read_file(FILE * fd,char** data);
		static uint64_t read_file(char* directory,char** data);
		static uint64_t read_file(int fd,char** data);

		static uint64_t prepare_media(char* directory,char** data);
		static string get_content_type(string directory);

		static void respond(int fd, char * data);
		static void respond(int fd, string data);
		static void respond(int fd, Response r);
		static void force_print(char* in,size_t length);
		static void append_to_file(string dir, string filename,string data);
		static void write_to_file(string dir, string filename, string data);
		static string read_from_file(string dir,string filename = "");
		static uint64_t read_from_file(string dir,string filename, string* data);
		static char* force_concat(char* s1,size_t l1,char*s2,size_t l2,uint8_t mem);
		static void create_tmp_file(char* directory);
};


#ifndef FILE_BUFFER_SIZE
#define FILE_BUFFER_SIZE 1000000
#endif

#ifndef WEB_ROOT
#define WEB_ROOT "./www"
#endif

#endif