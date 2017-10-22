#include "json.h"

using namespace std;

JSON::JSON(string t){
	if(t.compare("string") != 0){
		this->data = (JSON **)malloc(sizeof(JSON*));
		this->data[0] = nullptr;
	}
	this->type = t;
	this->values = new vector<void*>();
	this->name = "";
	this->url = "";
	this->keys = new vector<string>();
}

JSON* JSON::at(size_t i){
	return (JSON*)this->data[i];
}

JSON* JSON::at(string key){
	size_t length = this->size();
	for(int i = 0;i<length;i++){
		if(this->keys->at(i).compare(key) == 0){
			return (JSON*)this->data[i];
		}
	}
	return nullptr;
}

string* JSON::valueAt(string key){
	size_t length = this->keys->size();
	for(int i = 0;i<length;i++){
		if(this->keys->at(i).compare(key) == 0){
			return (string*)this->values->at(i);
		}
	}
	
	return nullptr;
}

string* JSON::valueAt(size_t index){		
	return (string*)this->values->at(index);
}

void JSON::add(string* element){
	if(this->type.compare("string")){
		cerr<<"ERROR, Cannot Add json. Wrong Type\n";
		exit(EXIT_FAILURE);
	}
	this->values->push_back((void*)element);
	this->keys->push_back(itoa(this->keys->size()));
}

void JSON::add(JSON* element){
	if(this->type.compare("JSON") != 0 && this->type.compare("json") != 0){
		cerr<<"ERROR, Cannot Add json\n";
		exit(EXIT_FAILURE);
	}
	size_t length = this->keys->size();
	this->data = (JSON **)realloc((void *)this->data,sizeof(JSON*)*(length+1));
	this->data[length] = element;
	this->keys->push_back(itoa(length));
}

void JSON::add(string key,string* element){
	if(this->type.compare("string") != 0){
		cout<<"ERROR, Cannot Add json. Wrong Type\n";
		exit(EXIT_FAILURE);
	}
	this->values->push_back(element);
	this->keys->push_back(key);
}

void JSON::add(string key, string element){
	this->add(key,new string(element));
}


void JSON::add(string key,JSON* element){
	size_t length = this->keys->size();
	if(this->type.compare("JSON") != 0 && this->type.compare("json") != 0){
		cerr<<"ERROR, Cannot Add json\n";
		exit(EXIT_FAILURE);
	}
	this->data = (JSON **)realloc((void *)this->data,sizeof(JSON)*(length+1));
	this->data[length] = element;
	this->keys->push_back(key);
}

string JSON::keyAt(size_t index){
	return this->keys->at(index);
}

size_t JSON::size(){
	return this->keys->size();
}

void JSON::addContents(vector<void*>* input){
	size_t length = input->size();
	for(int i = 0;i<length;i++){
		this->add((string*)input->at(i));
	}
}

string JSON::to_string(){
	string out = "";
	size_t length = this->size();
	if(length == 0){
		return "{}";
	}
	out = "{";
	if(this->type.compare("string") == 0){
		for(int i = 0;i<length;i++){
			out += "\"" + this->keyAt(i) + "\":";
			out += "\"" + *(this->valueAt(this->keyAt(i))) + "\"";
			if(i!=length-1){
				out += ",";
			}
		}
	}
	if(this->type.compare("JSON") == 0 || this->type.compare("json") == 0){
		for(int i = 0;i<length;i++){
			out += "\"" + this->keyAt(i) + "\":";
			out += this->at(this->keys->at(i))->to_string();
			if(i!=length-1){
				out += ",";
			}
		}
	}
	out += "}";
	return out;
}

