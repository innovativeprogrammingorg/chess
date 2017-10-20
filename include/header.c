#include "header.h"

Map parse_HTTP_body(char * body){
	Vector data = explode("&",body);
	size_t length = vector_length(data);
	Map out = NULL;
	uint_fast64_t i;
	Vector pair;
	char* value;
	char* tmp;
	for(i = 0;i<length;i++){
		pair = split('=',(char*)vector_get(data,i));
		
		tmp = (char*)vector_get(pair,1);
		value = (char*)calloc(sizeof(char*),strlen(tmp) + 1);

		strcpy(value,tmp);

		map_add(&out,vector_get(pair,0),value,STRING_TYPE);
		vector_clean(pair);
	}
	vector_clean(data);
	return out;
}

Map parse_HTTP_message(char * message){
	Map out = NULL;
	/*
	char* directory;
	char*  data;
	if(is_PHP_request(message,&directory)){
		map_add(&out,"SIZE",run_CGI(message,directory,&data),ULONGINT_TYPE);
		map_add(&out,"PHP_CGI",data,STRING_TYPE);
		return out;
	}*/
	if(strpos(message,"GET")==0){
		return parse_HTTP_header(message);
	}
	
	Vector v = ssplit("\r\n\r\n",message);
	if(vector_length(v)!=2){
		printf("Unexpected vector length of  %lu\n",vector_length(v));
		//exit(0);
	}
	Map header = parse_HTTP_header(vector_get(v,0));
	
	
	map_add(&out,"HEADER",header,MAP_TYPE);
	map_add(&out,"BODY",parse_HTTP_body(vector_get(v,1)),MAP_TYPE);
	vector_clean(v);
	return out;
}
Map parse_HTTP_header(char * header){
	/*puts("Parsing the HTTP header");*/
	//printf("\n\n\n\n%s\n\n\n\n",header);
	Vector parameters = explode("\r\n",header);
	size_t length = vector_length(parameters);
	uint_fast32_t i = 0;
	Map out = NULL;
	char* key;
	void* value;
	void* tmp;
	Vector arr;
	Vector key_value_pair = NULL;
	//puts("Parsing the parameters...");
	for(i = 0;i<length;i++){
		//printf("Parsing parameter %d/%lu\n",i,length);
		if(i == 0){
			key = "REQUEST";
			tmp = (char*)vector_get(parameters,i);
			value = calloc(sizeof(char*),strlen(tmp) + 1);
			strcpy((char*)value,tmp);
			map_add(&out,key,value,STRING_TYPE);
			continue;
		}
		key_value_pair = split(':',vector_get(parameters,i));

		key = trim((char*)vector_get(key_value_pair,0));
		value = trim((char*)vector_get(key_value_pair,1));
		
		//printf("Value is %s\n",(char*)value);
		if(strpos(key,"User-Agent")!=-1){
			map_add(&out,key,value,STRING_TYPE);
			//printf("Added %s:%s\n",key,(char*)value);
		}else if(indexOfChar((char*)value,';')!=-1){
			/*puts("Value is an array delim = ;");
			printf("Key is %s\n",key);*/
			arr = explode(";",(char*)value);
			free(value);
			value =  vector_to_array(arr);
			vector_free(arr);
			map_add(&out,key,value,ARRAY_TYPE);
		}else if(indexOfChar((char*)value,',')!=-1){
			/*puts("Value is an array delim = ,");
			printf("Key is %s\n",key);*/
			arr = explode(",",(char*)value);
			free(value);
			value =  vector_to_array(arr);
			vector_free(arr);
			map_add(&out,key,value,ARRAY_TYPE);
		}else{
			map_add(&out,key,value,STRING_TYPE);
			//printf("Added %s:%s\n",key,(char*)value);
		}
		free(key);
		vector_clean(key_value_pair);
		
	}
	vector_clean(parameters);
	return out;
}


char * get_status_line(int status){
	switch(status){
		case 200:
			return "HTTP/1.1 200 OK\r\n";
		case 404:
			return "HTTP/1.1 404 Not Found\r\n";
		case 400:
			return "HTTP/1.1 400 Bad Request\r\n";
		case 415:
			return "HTTP/1.1 415 Unsupported Media Type\r\n";
		case 403:
			return "HTTP/1.1 403 Forbidden\r\n";
		case 500:
			return "HTTP/1.1 500 Internal Server Error\r\n";
		case 503:
			return "HTTP/1.1 503 Service Unavailable\r\n";
	}
	return NULL;
}

char * get_date_line(){
    time_t timer;
    char* buffer = (char*)malloc(sizeof(char)*38);
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 37, "Date: %Y-%m-%d %H:%M:%S GMT\r\n", tm_info);
    return buffer;
}

char * get_server_line(){
	return "Server: Nate's C Server\r\n";
}

char* get_content_encoding_line(char * encode){
	return concat("Content-Encoding: ",concat(encode,"\r\n",FALSE),SECOND);
}

char* get_vary_line(){
	return "Vary: Accept-Encoding,User-Agent\r\n";
}

char* get_content_length_line(uint64_t content_length){
	char* out = (char*)calloc(sizeof(char),100);
	sprintf(out,"Content-Length: %lu\r\n",content_length);
	return out;
}

char* get_content_type_line(char* content){
	return concat("Content-Type: ",concat(content,"\r\n",FALSE),SECOND);
}

char* get_accept_ranges_line(){
	return "Accept-Ranges: bytes\r\n";
}

char* get_connection_line(Map header){
	return concat("Connection: ",concat((char*)map_value_at(header,"Connection"),"\r\n",FALSE),SECOND);
}

uint16_t check_valid_params(Map m){
	if(map_has_key(m,"HEADER")){
		return check_valid_params((Map)map_value_at(m,"HEADER"));
	}
	if(!map_has_key(m,"Host")){
		return 400;
	}
	if(!map_has_key(m,"Connection")){
		return 400;
	}
	if(!map_has_key(m,"REQUEST")){
		return 500;
	}

	return TRUE;
}

uint8_t get_request_type(Map m){
	char* status_line = (char*)map_value_at(m,"REQUEST");
	if(!status_line){
		status_line = (char*)map_value_at((Map)map_value_at(m,"HEADER"),"REQUEST");
	}
	if(!status_line){
		puts("An unknown error occured");
		return UNKNOWN;
	}
	if(strpos(status_line,"GET")==0){
		return GET;
	}
	if(strpos(status_line,"POST")==0){
		return POST;
	}
	if(strpos(status_line,"PUT")==0){
		return PUT;
	}
	if(strpos(status_line,"DELETE")==0){
		return DELETE;
	}
	if(strpos(status_line,"HEAD")==0){
		return HEAD;
	}
	if(strpos(status_line,"OPTIONS")==0){
		return OPTIONS;
	}
	if(strpos(status_line,"CONNECT")==0){
		return CONNECT;
	}
	return UNKNOWN;
}

Response build_response(Map m){
	Response out;
	/*if(map_has_key(m,"PHP_CGI")){
		return CGI_response(m);
	}*/
	if(1 != check_valid_params(m)){
		out = e400_response(m);
		map_clean(m);
		return out;
	}
	uint8_t request = get_request_type(m);
	switch(request){
		case GET:
			out = GET_response(m);
			break;
		case POST:
			puts("Received a POST request!");
			out =  POST_response(m);
			break;
		default:
			break;
	}
	map_clean(m);
	return out;
}	