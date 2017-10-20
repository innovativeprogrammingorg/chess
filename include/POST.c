#include "POST.h"

char* parse_PHP_args(Map args){
	char** keys = map_get_keys(args);
	char* out = NULL;
	uint64_t i = 0;
	while(keys[i]){
		out = concat(out,concat(concat(keys[i],concat("=",map_value_at(args,keys[i]),FALSE),FIRST|SECOND)," ",FIRST),(out)? FIRST | SECOND : SECOND);
		i++;
	}
	free(keys);
	return out;

}
char* process_through_PHP(Map _POST,char* dir){
	char* command = concat(concat(concat("/usr/bin/php -f ",dir,FALSE)," ",FIRST),parse_PHP_args(_POST),FIRST|SECOND);
	FILE* fd = popen(command,"r");
	char* body;
	if(!fread_file(fd,&body)){
		return NULL;
	}
	free(command);
	return body;
}