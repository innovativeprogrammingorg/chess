#ifndef json_H
#define json_H

#include <vector>
#include <stdlib.h>
#include <iostream>
#include "str.h"

using namespace std;

class JSON{
	public:
		vector<string>* keys;
		vector<void*>* values;
		JSON** data;
		string type;
		string name;
		string url;

		JSON(string t);
		JSON* at(size_t i);
		JSON* at(string key);
		string* valueAt(string key);
		string* valueAt(size_t index);
		
		void add(string* element);
		void add(JSON* element);
		void add(string key,string* element);
		void add(string key,JSON* element);
		void add(string key, string element);
		string keyAt(size_t index);
		size_t size();
		void addContents(vector<void*>* input);
		string to_string();
};



#endif
