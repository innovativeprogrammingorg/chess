#include "response.h"

Response new_response(char* header, char* body,size_t data_size){
	Response out = (Response)malloc(sizeof(struct response));
	out->header = header;
	out->body = body;
	out->data_size = data_size;
	return out;
}

Response e404_response(Map header){
	char* directory = "/404.shtml";
	char* body;
	char* head;
	uint64_t content_length;
	content_length = sread_file(directory,&body);
	head = get_status_line(404);
	head = concat(head,get_date_line(),SECOND);
	head = concat(head,get_server_line(),FIRST);
	head = concat(head,get_content_length_line(content_length),FIRST|SECOND);
	head = concat(head,get_connection_line(header),FIRST|SECOND);
	head = concat(head,get_content_type_line("text/html"),FIRST);
	head = concat(head,get_vary_line(),FIRST);
	//head = concat(head,get_content_encoding_line(),FIRST);
	return new_response(head,body,content_length);
}

Response e400_response(Map header){
	char* directory = "/400.shtml";
	char* body;
	char* head;
	uint64_t content_length;
	content_length = sread_file(directory,&body);
	head = get_status_line(404);
	head = concat(head,get_date_line(),SECOND);
	head = concat(head,get_server_line(),FIRST);
	head = concat(head,get_content_length_line(content_length),FIRST|SECOND);
	head = concat(head,get_connection_line(header),FIRST);
	head = concat(head,get_content_type_line("text/html"),FIRST|SECOND);
	head = concat(head,get_vary_line(),FIRST);
	//head = concat(head,get_content_encoding_line(),FIRST);
	return new_response(head,body,content_length);
}

Response GET_response(Map header){
	char* directory = get_requested_directory(header);
	char* body;
	char* head;
	uint64_t content_length;
	char* src;
	if(getLast(directory)=='/'){
		directory = concat(directory,"index.html",FIRST);
	}
	src = concat(WEB_ROOT,directory,FALSE);
	if(access(src,F_OK)==-1){
		return e404_response(header);
	}
	char* content_type = get_content_type(src);
	free(src);
	/**
	 * Build the HTTP Message
	 */
	head = get_status_line(200);
	if(strpos(content_type,"text")==-1){
		head = concat(head,get_accept_ranges_line(),FALSE);
		content_length = prepare_media(directory,&body);
		
		head = concat(head,get_content_type_line(content_type),FIRST|SECOND);
		head = concat(head,get_date_line(),FIRST|SECOND);
	}else{
		content_length = sread_file(directory,&body);
		head = concat(head,get_date_line(),SECOND);
	}
	//head = concat(head,get_content_encoding_line("gzip"),FIRST);
	head = concat(head,get_content_length_line(content_length),FIRST|SECOND);
	head = concat(head,get_connection_line(header),FIRST|SECOND);
	head = concat(head,get_server_line(),FIRST);
	free(directory);
	free(content_type);
	//head = concat(head,get_vary_line(),FIRST);
	return new_response(head,body,content_length);
}

Response POST_response(Map message){
	char* directory = get_requested_directory((Map)map_value_at(message,"HEADER"));
	directory = concat(WEB_ROOT,directory,SECOND);
	char* body = process_through_PHP(map_value_at(message,"BODY"),directory);
	if(body == NULL){
		return NULL;
	}
	size_t content_length = strlen(body);
	char* head = get_status_line(200);
	head = concat(head,get_date_line(),SECOND);
	head = concat(head,get_content_length_line(content_length),FIRST|SECOND);
	head = concat(head,get_connection_line(map_value_at(message,"HEADER")),FIRST|SECOND);
	head = concat(head,get_server_line(),FIRST);
	free(directory);
	return new_response(head,body,content_length);
}

char* get_requested_directory(Map m){
	char* status_line = (char*)map_value_at(m,"REQUEST");
	int start = indexOfChar(status_line,'/');
	int end = 0;
	while(status_line[end+start]!=' ' && status_line[end+start]!='\0' && status_line[end+start]!='?'){
		end++;
	}
	return substring(status_line,start,end);
}