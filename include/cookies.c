#include "cookies.h"

Cookie new_cookie(char* value,time_t created,time_t exp){
	Cookie out = (Cookie)malloc(sizeof(struct cookie));
	out->value = value;
	out->created = (created) ? created : time(NULL);
	out->expires = (exp) ? exp : time(NULL) + COOKIES_EXP;
	return out;
}

uint8_t store_cookie(char* ip, char* key, Cookie c){
	char* loc = concat(COOKIES_DIR,ip,FALSE);
	FILE* fd = fopen(loc,"a+");
	char * data = concat_all(8,key,"\035",c->value,"\035",ltoa((uint64_t)c->created),"\035",ltoa((uint64_t)c->expires),"\n");
	free(loc);
	fwrite(data,sizeof(char),strlen(data),fd);
	free(c);
	free(data);
	if(ferror(fd)){
		fclose(fd);
		return FALSE;
	}
	fclose(fd);
	return TRUE;
}

uint8_t store_cookies(char * ip, Map cookies){
	char** keys = map_get_keys(cookies);
	uint_fast64_t i = 0;
	char* loc = concat(COOKIES_DIR,ip,FALSE);
	uint8_t out = (uint8_t) (unlink(loc)== -1)? FALSE : TRUE;
	while(keys[i] && out){
		out = out & store_cookie(ip,keys[i],map_value_at(cookies,keys[i]));
		i++;
	}
	if(!out){
		return FALSE;
	}
	map_clean(cookies);
	free(loc);
	free(keys);
	return out;
}

Map load_cookies(char* ip){
	char* loc = concat(COOKIES_DIR,ip,FALSE);
	char* data;
	Vector lines;
	uint_fast32_t i;
	Map cookies = NULL;
	Vector cookie_dat;
	time_t current_time = time(NULL);
	time_t expires;
	int fd = open(loc,O_RDONLY,0);
	if(!read_file(fd,&data)){
		return NULL;
	}
	lines = explode("\n",data);
	free(data);
	for(i = 0;i<vector_length(lines);i++){
		cookie_dat = explode("\035",(char *)vector_get(lines,i));
		expires = atoi((char*)vector_get(cookie_dat,3));
		if(expires<current_time){
			continue;
		}
		map_add(&cookies,vector_get(cookie_dat,0),
			new_cookie((char*)vector_get(cookie_dat,1),atoi((char*)vector_get(cookie_dat,2)),expires),COOKIE_TYPE);
		vector_clean(cookie_dat);
	}
	free(loc);
	vector_clean(lines);
	return cookies;

}

uint8_t parse_cookies(char* ip,Map header){
	if(!map_has_key(header,"Cookie")){
		return TRUE;
	}
	Client c = find_client_by_ip(get_current_clients(),ip);
	Vector key_value;
	Map cookies = load_cookies(ip);
	char ** cookie_dat = (char**)map_value_at(header,"Cookie");
	uint_fast64_t i = 0;
	char* key;
	while(cookie_dat[i]){
		key_value = split('=',cookie_dat[i]);
		key = (char*)vector_get(key_value,0);
		if(map_has_key(cookies,key)){
			map_update(&cookies,key ,(char*)vector_get(key_value,1),STRING_TYPE);
		}else{
			map_add(&cookies, key,(char*)vector_get(key_value,1),STRING_TYPE);
		}
		i++;
	}

	c->cookies = cookies;

	return TRUE;
}





