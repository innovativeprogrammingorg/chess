#ifndef _HTTP_H_
#define _HTTP_H_

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include "str.h"

using namespace std;

class Header_Field{
	public:
		string** data;
		Header_Field(string** data);
		Header_Field(string* data);
		Header_Field(string data);
		virtual ~Header_Field();
		string to_string();
};
class HTTP_Header{
	public:
		map<string,Header_Field*>* data;
		HTTP_Header(string* header);
		virtual ~HTTP_Header();
		Header_Field* at(string* key);
		Header_Field* at(string key);
		bool has_param(string* key);
		bool has_param(string key);
		string to_string();
		
		static string** vector_to_array(vector<string>* in);
};

class HTTP_Request{
	public:
		map<string,string*>* body;
		HTTP_Header* header;

		HTTP_Request(string* data);
		virtual ~HTTP_Request();
		void parse_HTTP_body(string body);
		void parse_HTTP_body(char * body);
};



#endif