#ifndef _HTTP_H_
#define _HTTP_H_

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include "str.h"

using namespace std;

struct header_field{
	string ** data;
};

typedef struct header_field* Header_Field;

class HTTP_Header{
	public:
		map<string*,Header_Field> data;
		HTTP_Header(string* header);
		Header_Field at(string* key);
		Header_Field at(string key);
		bool has_param(string* key);
		bool has_param(string key);
		static Header_Field newHeader_Field(string** data);
		static Header_Field newHeader_Field(string* data);
		static Header_Field newHeader_Field(string data);
		static string** vector_to_array(vector<string>* in);
};

class HTTP_Request{
	public:
		map<string*,string*> body;
		HTTP_Header* header;

		HTTP_Request(string data);

		void parse_HTTP_body(char * body);
};



#endif