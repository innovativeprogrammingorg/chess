#include "io.h"

char* force_concat(char* s1,size_t l1,char*s2,size_t l2,uint8_t mem){
	size_t length = l1 + l2;
	char * out = (char *)calloc(sizeof(char),(length+1));
	register uint_fast64_t i;
	for(i = 0;i<l1;i++){
		out[i] = s1[i];
	}
	for(i = 0;i<l2;i++){
		out[i+l1] = s2[i];
	}
	if((mem >> 1)& 1){
		free(s2);
	}
	if((mem & 1)){
		free(s1);
	}
	out[length] = '\0';
	return out;
}

char * get_content_type(char* directory){
	char* command = concat("/usr/bin/file -i ",directory,FALSE);
	FILE* fd = popen(command,"r");
	char* file_type;
	Vector v;

	free(command);
	if(!fread_file(fd,&file_type)){
		puts("Error getting file type.\nAborting");
		exit(EXIT_FAILURE);
	}
	v = split(':',file_type);
	free(file_type);
	file_type = vector_get(v,1);
	if(indexOfChar(file_type,';')!=-1){
		file_type = substring(file_type,0,indexOfChar(file_type,';'));
	}
	vector_clean(v);
	printf("Requested item is of type: %s\n",file_type);
	return file_type;
}

void create_tmp_file(char* directory){
	puts("Creating the tmp file");
	char* src = concat(WEB_ROOT,directory,FALSE);
	char* dest = concat("./tmp",concat(directory,".gz",FALSE),SECOND);
	char * command = concat("gzip -c ",concat(src,concat(" > ",dest,SECOND),FIRST|SECOND),SECOND);
	system(command);
	free(command);
	puts("done");
}


uint64_t sread_file(char* directory,char** data){
	char buffer[FILE_BUFFER_SIZE];
	char* dat = NULL;
	size_t n;
	uint64_t out = 0;
	char* src = concat(WEB_ROOT,directory,FALSE);
	/*if(access(src,F_OK)==-1){
		create_tmp_file(directory);
	}*/
	FILE* fd =  fopen(src,"r");
	/*printf("reading the contents of the file...");*/
	while(!feof(fd)){
		memset(buffer,'\0',FILE_BUFFER_SIZE);
		n = fread(buffer,sizeof(char),FILE_BUFFER_SIZE - 1,fd);
		dat = concat(dat,buffer,(dat)? FIRST : FALSE);
		out += n;
	}
	*data = dat;
	printf("done\n");
	fclose(fd);
	free(src);
	return out;
}
uint64_t prepare_media(char* directory,char** data){
	//char* src = concat("./tmp",concat(directory,".gz",FALSE),SECOND);
	/*if(access(src,F_OK)==-1){
		create_tmp_file(directory);
	}*/
	char* src = concat(WEB_ROOT,directory,FALSE);
	FILE* fd =  fopen(src,"r");
	size_t n;
	char buffer[FILE_BUFFER_SIZE];
	char* dat = NULL;
	uint64_t out = 0;
	while(!feof(fd)){
		memset(buffer,'\0',FILE_BUFFER_SIZE);
		n = fread(buffer,sizeof(char),FILE_BUFFER_SIZE - 1,fd);
		dat = force_concat(dat,out,buffer,n,(dat != NULL)? FIRST : FALSE);
		out += n;	
	}
	*data = dat;
	fclose(fd);
	free(src);
	return out;
}

uint64_t fread_file(FILE* fd,char** data){
	char buffer[FILE_BUFFER_SIZE];
	char* dat = NULL;
	size_t n;
	uint64_t out = 0;
	while(!feof(fd)){
		memset(buffer,'\0',FILE_BUFFER_SIZE);
		n = fread(buffer,sizeof(char),FILE_BUFFER_SIZE - 1,fd);
		dat = concat(dat,buffer,(dat)? FIRST : FALSE);
		out += n;
		
	}
	*data = dat;
	fclose(fd);
	return out;
}

uint64_t read_file(int fd,char** data){
	char buffer[FILE_BUFFER_SIZE];
	char* dat = NULL;
	ssize_t n;
	uint64_t out = 0;
	memset(buffer,'\0',FILE_BUFFER_SIZE);

	while((n = read(fd,buffer,FILE_BUFFER_SIZE - 1))){
		dat = concat(dat,buffer,(dat)? FIRST : FALSE);
		out += n;
		memset(buffer,'\0',FILE_BUFFER_SIZE);
	}
	*data = dat;
	close(fd);
	return out;
}

void srespond(int fd, char* data){
	//printf("HTTP MESSAGE\n%s\n\n\n",data);
	int chunk_size = 1000;
	int64_t size = strlen(data); 
	int chunk = 0;
	char* buffer;
	printf("Sending %ld bytes of data to the client...",size);
	while(size>0){
		if(size>=chunk_size){
			buffer = substring(data,chunk*chunk_size,chunk_size);
			write(fd,buffer,chunk_size);
		}else{
			buffer = substring(data,chunk*chunk_size,size);
			write(fd,buffer,size);
		}
		free(buffer);
		size -=  chunk_size;
		chunk++;
	}
	puts("done");
	free(data);
}
void force_print(char* in,size_t length){
	uint64_t i;
	for(i = 0;i<length;i++){
		printf("%c",in[i]);
	}
	printf("\n");
}
void respond(int fd, Response r){
	
	int chunk_size = 1000;
	int64_t size = r->data_size;
	int chunk = 0;
	char* buffer;
	srespond(fd,concat(r->header,"\r\n",FALSE));
	printf("Sending %ld bytes of data to the client...",size);
	while(size>0){
		if(size>=chunk_size){
			buffer = substring(r->body,chunk*chunk_size,chunk_size);
			write(fd,buffer,chunk_size);
		}else{
			buffer = substring(r->body,chunk*chunk_size,size);
			write(fd,buffer,size);
		}
		free(buffer);
		size = size - chunk_size;
		chunk++;
	}
	puts("done");
	if(r->body){
		free(r->body);
	}
	if(r->header){
		free(r->header);
	}
	free(r);
	/*puts("DATA sent is ______________________________");
	force_print(r->body,r->data_size);
	puts("_____________________________");*/
}