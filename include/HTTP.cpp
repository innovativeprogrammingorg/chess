#include "HTTP.h"

using namespace std;
Header_Field HTTP_Header::newHeader_Field(string** data){
	Header_Field out = (Header_Field)malloc(sizeof(struct header_field));
	out->data = data;
	return out;
}

Header_Field HTTP_Header::newHeader_Field(string* data){
	Header_Field out = (Header_Field)malloc(sizeof(struct header_field));
	out->data = (string**)calloc(sizeof(string*),2);
	out->data[0] = data;
	out->data[1] = nullptr;
	return out;
}

Header_Field HTTP_Header::newHeader_Field(string data){
	out = HTTP_Header::newHeader_Field(new string(data));
	return out;
}

string ** HTTP_Header::vector_to_array(vector<string>* in){
	string** out = (string**)calloc(sizeof(string*),in->size() + 1);
	for(int i = 0;i<in->size();i++){
		out[i] = new string(in->at(i));
	}
	return out;
}

HTTP_Header::HTTP_Header(string header){
	vector<string>* parameters = explode("\r\n",header);
	size_t length = parameters->size();
	string key;
	Header_Field value;
	string* tmp;
	vector<string>* arr;
	vector<string>* key_value_pair;
	//puts("Parsing the parameters...");
	for(uint_fast32_t i = 0;i<length;i++){
		//printf("Parsing parameter %d/%lu\n",i,length);
		if(i == 0){
			key = new string("REQUEST");
			value = HTTP_Header::newHeader_Field(new string(parameters->at(i)));
			this->data->insert(pair<string*,Header_Field>(key,value));
			continue;
		}
		key_value_pair = split(':',parameters->at(i));

		key = new string(trim(key_value_pair->at(0)));
		tmp = new string(trim(key_value_pair->at(1)));
		
		//printf("Value is %s\n",(char*)value);
		if(key->find("User-Agent")!=string::npos){
			value = HTTP_Header::newHeader_Field(tmp);
			this->data->insert(pair<string*,Header_Field>(key,value));
			//printf("Added %s:%s\n",key,(char*)value);
		}else if(tmp->find(";")!=string::npos){
			/*puts("Value is an array delim = ;");
			printf("Key is %s\n",key);*/

			arr = explode(";",*tmp);
			value =  HTTP_Header::newHeader_Field(HTTP_Header::vector_to_array(arr));
			this->data->insert(pair<string*,Header_Field>(key,value));
			delete arr;
		}else if(tmp->find(",")!=string::npos){
			/*puts("Value is an array delim = ,");
			printf("Key is %s\n",key);*/
			arr = explode(",",*tmp);
			value =  HTTP_Header::newHeader_Field(HTTP_Header::vector_to_array(arr));
			this->data->insert(pair<string*,Header_Field>(key,value));
			delete arr;
		}else{
			value = HTTP_Header::newHeader_Field(tmp);
			this->data->insert(pair<string*,Header_Field>(key,value));
			//printf("Added %s:%s\n",key,(char*)value);
		}
		delete key_value_pair;
		
	}
	delete parameters;
}

Header_Field HTTP_Header::at(string* key){
	return this->data->at(key);
}

Header_Field HTTP_Header::at(string key){
	string* k = new string(key);
	Header_Field out = this->data->at(k);
	delete k;
	return out;
}

bool HTTP_Header::has_param(string* key){
	try{
		this->data->at(key);
		return true;
	}catch(const out_of_range& oor){
		return false;
	}
}

bool HTTP_Header::has_param(string key){
	string* k = new string(key);
	try{
		this->data->at(k);
		delete k;
		return true;
	}catch(const out_of_range& oor){
		delete k;
		return false;
	}
}


void HTTP_request::parse_HTTP_body(string body){
	vector<string>* data = explode("&",body);
	size_t length = data->size();
	
	this->body = new map<string*,string*>();
	vector<string>* pair;
	string* value;
	string* key;
	for(uint_fast64_t i = 0;i<length;i++){
		pair = split('=',data->at(i));
		
		key = new string(pair->at(0));
		value = new string(pair->at(1));
		this->body->insert(pair<string*,string*>(key,value));
		delete pair;
	}
	delete data;
	return out;
}



HTTP_request::HTTP_request(string message){
	/*
	char* directory;
	char*  data;
	if(is_PHP_request(message,&directory)){
		map_add(&out,"SIZE",run_CGI(message,directory,&data),ULONGINT_TYPE);
		map_add(&out,"PHP_CGI",data,STRING_TYPE);
		return out;
	}
	if(strpos(message,"GET")==0){
		return parse_HTTP_header(message);
	}*/
	try{
		vector<string>* v = ssplit("\r\n\r\n",message);
		if(v->size()!=2){
			printf("Unexpected vector length of  %lu\n",vector_length(v));
		//exit(0);
		}
		this->header = new Header(v->at(0));
		this->parse_HTTP_body(v->at(1));
		delete v;
	}catch(const out_of_range& oor){
		this->header = new Header(message);
		this->body = nullptr;
	}
}