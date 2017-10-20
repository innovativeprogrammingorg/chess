#include "header.h"

void server_handshake(char* request,int fd){
	Vector fields = explode("\r\n",request);
	size_t length = vector_length(fields);
	printf("There are %lu fields in the header\n",length);
	int req = request_type(vector_get(fields,0));
	JSON header = parseRequest(fields);
	char * response = generateResponse(req,header,fields);
	printf("%s\n",response);
	puts("Sending the response to the client");
	send(fd,response,strlen(response),0);
	puts("Sent");
	puts("Ending the handshake!\n");
}


int request_type(char* header){
	if(strpos(header,"GET")!= -1){
		return GET;
	}
	if(strpos(header,"POST")!=-1){
		return POST;
	}
	if(strpos(header,"UPDATE")!=-1){
		return UPDATE;
	}
	if(strpos(header,"DELETE")!=-1){
		return DELETE;
	}
}

JSON parseRequest(Vector header){
	size_t i = 0;
	JSON head = createJSON("string");
	size_t length = vector_length(header);
	int index;
	char* data = NULL;
	for(i = 1;i<length;i++){
		data = (char *)vector_get(header,i);
		index = indexOfChar(data,':');
		if(index == -1){
			puts("unable to find the character!");
			continue;
		}
		//printf("%s \n%s\n",substring(data,0,index),substr(data,index+2));
		addss(head,substring(data,0,index),substr(data,index+2));
	}
	return head;
}

char* convToBase64(uint8_t* hex,size_t ilength){
	int size = 0;
	return base64(hex,ilength,&size);
}

char* sha1(char* str){
    FILE *fp;
    int BSIZE = 300;
    char path[BSIZE+1];
    char * tmp = "php -r \"echo sha1(\'";
    tmp = concat(tmp,str,FALSE);
    tmp = concat(tmp,"\');\"",FIRST);
    fp = popen(tmp, "r");
    if(fp == NULL){
        printf("Failed to run command\n" );
        exit(EXIT_FAILURE);
    }
    char * out = (char *)malloc(sizeof(char)*(BSIZE+1));
    fgets(out, BSIZE, fp);
    int count = 1;
    while(fgets(path, sizeof(path)-1, fp) != NULL){
        count++;
        out = (char *)realloc(out,count*BSIZE*sizeof(char)+1);
        out = strcat(out,path);
    }
    pclose(fp);
    out[count*BSIZE+1] = '\0';
    return out;
}

char bcode(uint8_t in){
	if(in<26){
		return 65 + in; //A-Z
	}
	if(in<52){
		return 71 + in; //a-z
	}
	if(in<62){
		return in - 4;
	}
	if(in==62){
		return '+';
	}else{
		return '/';
	}
}

char* base64_encode(char* str){
	char* out;
	uint8_t* buffer;
	size_t length_64;
	size_t length = strlen(str);
	size_t remainder = (8*length) % 3;
	length_64 = (4*length)/3;
	length_64 += (remainder==0)? 0 :1;
	buffer = (uint8_t*)calloc(length_64,sizeof(uint8_t));
	size_t i = 0;
	size_t bit = 0;
	size_t pos = 0;
	uint8_t tmp;
	for(i = 0;i<length;i++){
		for(int j = 7;j>=0;j--){
			buffer[pos] += (((str[i]>>j)&1) << (5- bit));
			bit++;
			if(bit==6){
				pos++;
				bit = 0;
			}
		}
	}
	out = (char*)calloc(length_64+remainder+1,sizeof(char));
	for(i=0;i<length_64;i++){
		out[i] = bcode(buffer[i]);
	}
	if(remainder == 0){
		return out;
	}
	if(remainder>0){
		out[length_64] = '=';
	}
	if(remainder>1){
		out[length_64+1] = '=';
	}
	return out;
}

int hexdec(char* in){
	char hex[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	int i = 0;
	int j = 0;
	int k = 0;
	for(k = 0;k<16;k++){
		if(hex[k]==in[0]){
			i = k;
		}
		if(hex[k]==in[1]){
			j = k;
		}
	}
	return (i*16) + j;
}

char* sec_webkey(char* str){
	char* hash = sha1(str);
	char hashdec[5000];
	size_t max = strlen(hash);
	size_t index = 0;
	for (size_t i = 0; i < max; i += 2) { 
	    hashdec[index] = hexdec(substring(hash, i, 2)); 
	    index++;
	}
	hashdec[index]='\0';
	return base64_encode(hashdec);
}

char* generateResponse(int request, JSON header,Vector fields){
	char* out = NULL;
	char* status;
	char buffer[3000];
	int length;
	char* key;
	printf("Generating a response\n");
	char* salt = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	if(request == GET){
		status = "HTTP/1.1 101 Switching Protocols\r\n";
		out = concat(out,status,FALSE);
	}
	printf("adding the host to the reponse\n");
	length = sprintf((char *)buffer,"Host: %s\r\n",valueAtc(header,"Host"));
	buffer[length] = '\0';
	out = concat(out,buffer,FIRST);
	memset(buffer,0,length);

	length = sprintf((char*)buffer,"Connection: %s\r\n",valueAtc(header,"Connection"));
	buffer[length] = '\0';
	out = concat(out,buffer,FIRST);
	memset(buffer,0,length);

	length = sprintf((char*)buffer,"Upgrade: %s\r\n",valueAtc(header,"Upgrade"));
	buffer[length] = '\0';
	out = concat(out,buffer,FIRST);
	memset(buffer,0,length);

	length = sprintf((char*)buffer,"Sec-WebSocket-Protocol: %s\r\n",valueAtc(header,"Sec-WebSocket-Protocol"));
	buffer[length] = '\0';
	out = concat(out,buffer,FIRST);
	memset(buffer,0,length);
	//out = concat(out,"Content-Type: text/plain; charset=UTF-8\r\n",FIRST);
	out = concat(out,"Pragma: no-cache\r\n",FIRST);
	out = concat(out,"Cache-Control: no-cache\r\n",FIRST);

	puts("Now creating the response key!");
	key = valueAtc(header,"Sec-WebSocket-Key");
	puts("Found the response key");
	key = concat(key,salt,FALSE);
	printf("The concat gives %s\n",key);
	key = sec_webkey(key);
	printf("The result is %s\n",key);
	sprintf(buffer,"%s\r\n\r\n",concat("Sec-WebSocket-Accept: ",key,FALSE));
	out = concat(out,buffer,FIRST);
	return out;
}