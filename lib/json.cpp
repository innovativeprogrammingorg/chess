#include "json.h"


JSON ati(JSON j,size_t i){
	return j->data[i];
}

JSON atc(JSON j, char * key){
	size_t index = 0;
	int i;
	size_t length = size(j);
	for(i = 0;i<length;i++){
		if(strcompare(vector_get(j->keys,i),key)){
			index = i;
			break;
		}
	}
	
	return j->data[index];
}

char * valueAtc(JSON j, char * key){
	int index = 0;
	size_t length = vector_length(j->keys);
	int i;
	for(i = 0;i<length;i++){
		if(strcompare(vector_get(j->keys,i),key)){
			index = i;
			break;
		}
	}
	
	return vector_get(j->values,index);
}

char * valueAti(JSON j, size_t index){		
	return vector_get(j->values,index);
}

JSON createJSON(char * t){
	JSON out = (JSON)malloc(sizeof(struct json));
	if(!strcompare(t,"string")){
		out->data = (JSON *)malloc(sizeof(JSON));
	}
	out->type = t;
	out->values = NULL;
	out->name = NULL;
	out->url = NULL;
	out->keys = NULL;
	return out;
}


void adds(JSON j, char * element){
	if(!strcompare(j->type,"string")){
		printf("ERROR, Cannot Add json. Wrong Type\n");
		exit(EXIT_FAILURE);
	}
	vector_push(&(j->values),element);
	vector_push(&(j->keys),itoa(vector_length(j->keys)));
}

void addj(JSON j,JSON element){
	if(!strcompare(j->type,"JSON")&& !strcompare(j->type,"json")){
		printf("ERROR, Cannot Add json\n");
		exit(EXIT_FAILURE);
	}
	size_t length = vector_length(j->keys);
	j->data = (JSON *)realloc((void *)j->data,sizeof(JSON)*(length+1));
	j->data[length] = element;
	vector_push(&(j->keys),itoa(length));
}

void addss(JSON j,char * key,char * element){
	if(!strcompare(j->type,"string")){
		printf("ERROR, Cannot Add json. Wrong Type\n");
		exit(EXIT_FAILURE);
	}
	vector_push(&(j->values),element);
	vector_push(&(j->keys),key);
}


void addsj(JSON j, char * key,JSON element){
	size_t length = vector_length(j->keys);
	if(!strcompare(j->type,"JSON")&& !strcompare(j->type,"json")){
		printf("ERROR, Cannot Add json\n");
		exit(EXIT_FAILURE);
	}
	j->data = (JSON *)realloc((void *)j->data,sizeof(JSON)*(length+1));
	j->data[length] = element;
	vector_push(&(j->keys),key);
}

char * keyAt(JSON j, size_t index){
	return vector_get(j->keys,index);
}

size_t size(JSON j){
	return vector_length(j->keys);
}

void addContents(JSON j, Vector input){
	size_t length = vector_length(input);
	int i;
	for(i = 0;i<length;i++){
		adds(j,vector_get(input,i));
	}
}

char * jsonToString(JSON data){
	char * out = NULL;
	char * type = data->type;
	size_t length = size(data);
	register uint_fast64_t i = 0;
	if(length == 0){
		out = (char *)malloc(sizeof(char)*3);
		out[0] = '{';
		out[1] = '}';
		out[2] = '\0';
		return out;
	}
	out = (char *)malloc(sizeof(char) * 2);
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

