#include "./include/server.h"

int stringToInt(char *s){
	int i = 0;
	int out = 0;
	int length = 0;
	while(s[length]!='\0'){
		length++;
	}
	while(i<length){
		out = out + (s[i]-48)* expo(10,length-(i+1));
		i++;
	}
	return out;
}