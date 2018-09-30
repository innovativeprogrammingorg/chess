#ifndef _HTTP_H_
#define _HTTP_H_

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>
#include <iostream>
#include "str.h"

using namespace std;

class header_field{
	public:
		vector<string> data;
		Header_Field(const vector<string>& data);
		Header_Field(const string& data);
		string to_string();
};

class http_header{
	public:
		map<string,header_field> data;
		http_header(const string& header);
		virtual ~http_header();
		Header_Field& at(const string& key);
		bool has_param(const string& key);
		string to_string();
};

class http_request{
	public:
		map<string,string> body;
		http_header header;

		http_request(const string& data);
		void parse_http_body(const string& body);
		void parse_http_body(char* body);
};



#endif