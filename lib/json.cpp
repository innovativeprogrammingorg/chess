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
	return j->data[i];
}

JSON* JSON::at(string key){
	size_t length = this->size();
	for(int i = 0;i<length;i++){
		if(this->keys->at(i).compare(key) == 0){
			return j->data[i];
		}
	}
	return nullptr;
}

string JSON::valueAt(string key){
	int index = 0;
	size_t length = this->keys->size();
	for(int i = 0;i<length;i++){
		if(this->keys->at(i).compare(key) == 0){
			index = i;
			break;
		}
	}
	
	return vector_get(j->values,index);
}

string JSON::valueAt(size_t index){		
	return j->values->at(index);
}

void JSON::add(string element){
	if(!strcompare(j->type,"string")){
		cerr<<"ERROR, Cannot Add json. Wrong Type\n";
		exit(EXIT_FAILURE);
	}
	j->values->push_back((void*)element);
	vector_push(&(j->values),element);
	vector_push(&(j->keys),itoa(vector_length(j->keys)));
}

void JSON::add(JSON j,JSON element){
	if(!strcompare(j->type,"JSON")&& !strcompare(j->type,"json")){
		printf("ERROR, Cannot Add json\n");
		exit(EXIT_FAILURE);
	}
	size_t length = vector_length(j->keys);
	j->data = (JSON *)realloc((void *)j->data,sizeof(JSON)*(length+1));
	j->data[length] = element;
	vector_push(&(j->keys),itoa(length));
}

void JSON::add(JSON j,string key,string element){
	if(this->type.compare("string") != 0){
		cout<<"ERROR, Cannot Add json. Wrong Type\n";
		exit(EXIT_FAILURE);
	}
	vector_push(&(j->values),element);
	vector_push(&(j->keys),key);
}


void JSON::add(string key,JSON element){
	size_t length = this->keys->size();
	if(j->type.compare("JSON") != 0 && !strcompare(j->type,"json")){
		cerr<<"ERROR, Cannot Add json\n";
		exit(EXIT_FAILURE);
	}
	j->data = (JSON *)realloc((void *)j->data,sizeof(JSON)*(length+1));
	j->data[length] = element;
	vector_push(&(j->keys),key);
}

string JSON::keyAt(JSON j, size_t index){
	return vector_get(j->keys,index);
}

size_t JSON::size(){
	return j->keys->size();
}

void JSON::addContents(vector<void*>* input){
	size_t length = vector_length(input);
	for(int i = 0;i<length;i++){
		adds(j,vector_get(input,i));
	}
}

string JSON::to_string(){
	string out = NULL;
	string type = data->type;
	size_t length = size(data);
	int i = 0;
	if(length == 0){
		out = (string)malloc(sizeof(char)*3);
		out[0] = '{';
		out[1] = '}';
		out[2] = '\0';
		return out;
	}
	out = (string)malloc(sizeof(char) * 2);
	out[0] = '{';
	out[1] = '\0';
	if(strcompare(type,"string")){
		for(i = 0;i<length;i++){
			out = concat(concat(out,concat("\"",concat(keyAt(data,i),"\":",FALSE),SECOND),FIRST|SECOND),
					  concat("\"",concat(valueAtc(data,keyAt(data,i)),"\"",FALSE),SECOND),FIRST|SECOND);
			
			if(i!=length-1){
				out = concat(out,",",FIRST);
			}
		}
	}
	if(strcompare(type,"json")||strcompare(type,"JSON")){
		for(i = 0;i<length;i++){
			out = concat(concat(out,concat("\"",concat(keyAt(data,i),"\":",FALSE),SECOND),FIRST|SECOND),
					jsonToString(atc(data,vector_get(data->keys,i))),FIRST|SECOND);
			if(i!=length-1){
				out = concat(out,",",FIRST);
			}
		}
	}
	out = concat(out,"}", FIRST);
	return out;
}

