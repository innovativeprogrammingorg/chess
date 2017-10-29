#include "HTTP.h"

using namespace std;

Header_Field::Header_Field(string** data){
	this->data = data;
}

Header_Field::Header_Field(string* data){
	this->data = (string**)calloc(sizeof(string*),2);
	this->data[0] = data;
	this->data[1] = nullptr;
}

Header_Field::Header_Field(string data){
	this->data = (string**)calloc(sizeof(string*),2);
	this->data[0] = new string(data);
	this->data[1] = nullptr;
}

Header_Field::~Header_Field(){
	uint_fast64_t i = 0;
	while(this->data[i]!=nullptr){
		delete this->data[i];
		i++;
	}
	free(this->data);
}

string Header_Field::to_string(){
	int_fast64_t i = 0;
	string out = "";
	while(this->data[i]!= nullptr){
		out += "["+ltos((int64_t)i) + "]: " + *this->data[i] + "\n";
		i++;
	}
	return out;
}


string ** HTTP_Header::vector_to_array(vector<string>* in){
	string** out = (string**)calloc(sizeof(string*),in->size() + 1);
	for(int i = 0;i<in->size();i++){
		out[i] = new string(in->at(i));
	}
	out[in->size()] = nullptr;
	return out;
}

string HTTP_Header::to_string(){
	string out;
	for(map<string,Header_Field*>::iterator it = this->data->begin();it!=this->data->end();++it){
		out.append(it->first);
		out.append(":{ \n");
		out.append(it->second->to_string());
		out.append("}\n");
	}

	return out;
}

HTTP_Header::HTTP_Header(string* header){
	this->data = new map<string,Header_Field*>();
	vector<string>* parameters = explode("\r\n",*header);
	size_t length = parameters->size();
	string key;
	Header_Field* value;
	string* tmp;
	vector<string>* arr;
	vector<string>* key_value_pair;
	//cout<<"Parsing the parameters..."<<endl;
	for(uint_fast32_t i = 0;i<length;i++){
		//cout<<"Parsing parameter "<<i<<"/"<<length<<endl;
		if(i == 0){
			key = "REQUEST";
			value = new Header_Field(new string(parameters->at(i)));
			this->data->insert(pair<string,Header_Field*>(key,value));
			continue;
		}
		key_value_pair = split(':',parameters->at(i));

		key = trim(key_value_pair->at(0));
		tmp = new string(trim(key_value_pair->at(1)));
		
		//cout<<"Value is "<<*tmp<<endl;
		if(key.find("User-Agent")!=string::npos){
			value = new Header_Field(tmp);
			this->data->insert(pair<string,Header_Field*>(key,value));
			//printf("Added %s:%s\n",key,(char*)value);
		}else if(tmp->find(";")!=string::npos){
			/*puts("Value is an array delim = ;");
			printf("Key is %s\n",key);*/

			arr = explode(";",*tmp);
			value =  new Header_Field(HTTP_Header::vector_to_array(arr));
			this->data->insert(pair <string,Header_Field*> (key,value));
			delete arr;
		}else if(tmp->find(",")!=string::npos){
			/*puts("Value is an array delim = ,");
			printf("Key is %s\n",key);*/
			arr = explode(",",*tmp);
			value =  new Header_Field(HTTP_Header::vector_to_array(arr));
			this->data->insert(pair<string,Header_Field*>(key,value));
			delete arr;
		}else{
			value = new Header_Field(tmp);
			this->data->insert(pair<string,Header_Field*>(key,value));
			//printf("Added %s:%s\n",key,(char*)value);
		}
		delete key_value_pair;
		
	}
	delete parameters;
}


HTTP_Header::~HTTP_Header(){
	if(this->data!=nullptr ){
		delete this->data;
	}
	
}

Header_Field* HTTP_Header::at(string key){
	return this->data->at(key);
}

Header_Field* HTTP_Header::at(string* key){
	this->at(*key);
}

bool HTTP_Header::has_param(string* key){
	try{
		this->data->at(*key);
		return true;
	}catch(const out_of_range& oor){
		return false;
	}
}

bool HTTP_Header::has_param(string key){
	try{
		this->data->at(key);
		return true;
	}catch(const out_of_range& oor){
		return false;
	}
}

void HTTP_Request::parse_HTTP_body(char* body){
	this->parse_HTTP_body(string(body));
}
void HTTP_Request::parse_HTTP_body(string body){
	vector<string>* data = explode("&",body);
	size_t length = data->size();
	
	this->body = new map<string,string*>();
	vector<string>* kv_pair;
	string* value;
	string key;
	for(uint_fast64_t i = 0;i<length;i++){
		kv_pair = split('=',data->at(i));
		
		key = kv_pair->at(0);
		value = new string(kv_pair->at(1));
		this->body->insert(pair<string,string*>(key,value));
		delete kv_pair;
	}
	delete data;
}



HTTP_Request::HTTP_Request(string* message){
	this->header = nullptr;
	this->body = nullptr;
	/*
	char* directory;
	char*  data;
	if(is_PHP_request(message,&directory)){
		map_add(&out,"SIZE",run_CGI(message,directory,&data),ULONGINT_TYPE);
		map_add(&out,"PHP_CGI",data,STRING_TYPE);
		return out;
	}*/
	if(message->find("GET") == 0|| message->find("\r\n\r\n") == string::npos){
			this->header = new HTTP_Header(message);
	}else{
		try{
			//cout<<"HTTP message has message data to be read"<<endl;
			vector<string>* v = ssplit("\r\n\r\n",*message);
			if(v->size()!=2){
				printf("Unexpected vector length of  %lu\n",v->size());
			//exit(0);
			}
			this->header = new HTTP_Header(new string(v->at(0)));
			this->parse_HTTP_body(v->at(1));
			delete v;
		}catch(const out_of_range& oor){
			this->header = new HTTP_Header(message);
			this->body = nullptr;
		}
	}
	
}

HTTP_Request::~HTTP_Request(){
	if(this->header != nullptr){
		delete this->header;
	}
	if(this->body != nullptr){
		delete this->body;
	}
}