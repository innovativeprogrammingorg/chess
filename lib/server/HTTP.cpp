#include "server/HTTP.h"

using namespace std;

header_field::header_field(const vector<string>& data){
	this->data = data;
}


header_field::header_field(const string& data){
	this.data.push_back(data);
}


string header_field::to_string(){
	
	string out = "";
	for(int_fast64_t i = 0;i < this->data.size();i++){
		out += "[" + to_string(i) + "]: " + this->data[i] + "\n";
	}
	return out;
}


string http_header::to_string(){
	string out;
	for(auto it = this->data.begin();it!=this->data.end();++it){
		out.append(it->first);
		out.append(":{ \n");
		out.append(it->second.to_string());
		out.append("}\n");
	}

	return out;
}

http_header::http_header(const string& header){
	
	vector<string> parameters = explode("\r\n",header);
	size_t length = parameters.size();
	string key;
	header_field value;
	string tmp;
	vector<string> arr;
	vector<string> key_value_pair;
	//cout<<"Parsing the parameters..."<<endl;
	for(uint_fast32_t i = 0;i<length;i++){
		//cout<<"Parsing parameter "<<i<<"/"<<length<<endl;
		if(i == 0){
			key = "REQUEST";
			this->data.emplace(key,header_field(parameters.at(i)));
			continue;
		}
		key_value_pair = split(':',parameters.at(i));

		key = trim(key_value_pair->at(0));
		tmp = trim(key_value_pair->at(1));
		
		//cout<<"Value is "<<*tmp<<endl;
		if(key.find("User-Agent")!=string::npos){
			this->data->emplace(key,header_field(tmp));
			//printf("Added %s:%s\n",key,(char*)value);
		}else if(tmp.find(";")!=string::npos){
			/*puts("Value is an array delim = ;");
			printf("Key is %s\n",key);*/

			arr = explode(';',tmp);
			this->data->emplace(key,make_unique<header_field>(arr));
			
		}else if(tmp.find(',')!=string::npos){
			/*puts("Value is an array delim = ,");
			printf("Key is %s\n",key);*/
			arr = explode(',',tmp);
			this->data.emplace(key,header_field(arr));
			d
			
		}else{
			this->data.emplace(key,header_field(tmp));
			//printf("Added %s:%s\n",key,(char*)value);
		}
		delete key_value_pair;
	}
	delete parameters;
}

header_field& http_header::at(const string& key){
	return this->at(key);
}


bool http_header::has_param(const string& key){
	try{
		this->data->at(key);
		return true;
	}catch(const out_of_range& oor){
		return false;
	}
}

void http_request::parse_http_body(char* body){
	this->parse_http_body(string(body));
}
void http_request::parse_http_body(const string& body){
	vector<string> data = explode('&',body);
	size_t length = data.size();
	
	for(uint_fast64_t i = 0;i<length;i++){
		vector<string> kv_pair = split('=',data.at(i));
		this->body.emplace(kv_pair->at(0),kv_pair.at(1));
	}

}



http_request::http_request(const string& message){
	/*
	char* directory;
	char*  data;
	if(is_PHP_request(message,&directory)){
		map_add(&out,"SIZE",run_CGI(message,directory,&data),ULONGINT_TYPE);
		map_add(&out,"PHP_CGI",data,STRING_TYPE);
		return out;
	}*/
	if(message.find("GET") == 0|| message.find("\r\n\r\n") == string::npos){
			this->header = http_header(message);
	}else{
		try{
			//cout<<"HTTP message has message data to be read"<<endl;
			vector<string> v = split("\r\n\r\n",*message);
			if(v->size()!=2){
				printf("Unexpected vector length of  %lu\n",v.size());
			//exit(0);
			}
			this->header = http_header(v.at(0));
			this->parse_http_body(v.at(1));
			delete v;
		}catch(const out_of_range& oor){
			this->header = http_header(message);
		}
	}
	
}