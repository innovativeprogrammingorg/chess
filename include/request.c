#include "request.h"

Request new_request(Client c, char* data){
	Request out = (Request)malloc(sizeof(struct request));
	out->client = c;
	out->data = data;
	return out;
}

uint8_t is_PHP_request(char * message, char** directory){
	int i = strpos(message,"\r\n");
	if(i == -1){
		puts("Unexpected error in is_PHP_request");
		exit(EXIT_FAILURE);
	}
	char* first_line = substring(message,0,i);
	int start = indexOfChar(first_line,'/');
	int end = 0;
	while(first_line[end+start]!=' ' && first_line[end+start]!='\0' && first_line[end+start]!='?'){
		end++;
	}
	char* dir = concat("./www",substring(first_line,start,end),SECOND);
	free(first_line);
	char* type = get_content_type(dir);
	if(strpos(type,"php")!= -1){
		*directory = dir;
		return TRUE;
	}
	free(dir);
	*directory = NULL;
	return FALSE;
}
