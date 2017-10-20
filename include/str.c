#include "str.h"

Vector explode(char * quan,char * subject){
	Vector out = NULL;
	char * subj;
	register uint_fast64_t i;                                             
	size_t qlength = strlen(quan);
	size_t slength = strlen(subject);
	subj = (char *)malloc(sizeof(char)*(slength+1));
	strcpy(subj,subject);
	subj[slength] = '\0';
	for(i = 0;i<slength-qlength;i++){
		if(strcompsub(quan,subj,i,qlength)){
			vector_push(&out,substring(subj,0,i));
			subj = substr_f(subj,i+1);
			i = 0;
			slength = strlen(subj);
		}
	}
	free(subj);
	return out;
}

Vector split(char quan,char * subject){
	Vector out = NULL;
	int index = indexOfChar(subject,quan);
	vector_push(&out,substring(subject,0,index));
	vector_push(&out,substr(subject,index+1));
	return out;
}

Vector ssplit(char* quan,char * subject){
	Vector out = NULL;
	int index = strpos(subject,quan);
	vector_push(&out,substring(subject,0,index));
	vector_push(&out,substr(subject,index+strlen(quan)));
	return out;
}

char * substring_f(char * subject,size_t index,size_t length){
	char * out = substring(subject,index,length);
	free(subject);
	return out;
}

char * substr_f(char * subject,size_t index){
	char * out = substr(subject,index);
	free(subject);
	return out;
}

char* itoa(uint32_t num){
	char * characters[10] = {"0","1","2","3","4","5","6","7","8","9"};
	return (num>=10)? concat(itoa(num/10),characters[num%10],FALSE) : characters[num];
}

char* ltoa(uint64_t num){
	char * characters[10] = {"0","1","2","3","4","5","6","7","8","9"};
	return (num>=10)? concat(itoa(num/10),characters[num%10],FALSE) : characters[num];
}

char* str_reverse(char* str){
	size_t length = strlen(str);
	char * out = (char*)malloc(sizeof(char)*(length+1));
	size_t i = 0;
	for(i = 0;i<length;i++){
		out[i] = str[(length-1)-i];
	}
	out[length] = '\0';
	free(str);
	return out;
}

char* strappend(char s,char * str){
	size_t length = strlength(str);
	char* out = (char*)malloc(sizeof(char)*(length+2));
	register size_t i;
	out[0] = s;
	if(str==NULL){
		out[1] = '\0';
		return out;
	}
	for(i = 1;i<length+2;i++){
		out[i] = str[i-1];
	}
	free(str);
	out[length + 1] = '\0';
	return out;
}

char* stradd(char * str,char s){
	size_t length = strlength(str);
	char* out = (char*)malloc(sizeof(char)*(length+2));
	if(str!=NULL){
		strcpy(out,str);
		free(str);
	}
	out[length + 1] = '\0';
	out[length] = s;
	return out;
}


char getLast(char * str){
	return str[strlen(str)-1];
}

char lastChar(char * str){
	return str[strlen(str)-1];
}

char * substring(char * subject,size_t index,int length){
	if(length<0){
		return substring(subject,index,(strlen(subject) - (index - length)));
	}
	char * out = (char *)malloc(sizeof(char)*(length + 1));
	register uint_fast64_t i;

	for(i = 0;i<length;i++){
		out[i] = subject[i+index];
	}
	out[length] = '\0';
	return out;
}


char* substr_r(char * subject,int index){
	uint64_t length = strlen(subject);
	char * out;
	register uint_fast64_t i = 0;
	register uint_fast64_t start = length + index;
	out = (char *)malloc(sizeof(char)*(index*-1 + 1));

	while(subject[i+start]!='\0'){
		out[i] = subject[i+start];
		i++;
	}
	out[i] = '\0';
	return out;
}

char* substr(char* subject,int index){
	if(index<0){
		return substr_r(subject,index);
	}
	uint64_t length = strlength(subject);
	char * out;
	register uint_fast64_t i = 0;
	if(length<index+1){
		return NULL;
	}
	out = (char *)malloc(sizeof(char)*(length-index + 1));
	while(subject[i+index]!='\0'){
		out[i] = subject[i+index];
		i++;
	}
	out[i] = '\0';
	return out;
}

uint_least8_t strcompare(char * str1, char * str2){
	register uint64_t l1 = strlen(str1);
	register uint_fast64_t i;
	if(l1 != strlen(str2)){
		return FALSE;
	}
	for(i = 0;i<l1;i++){
		if(str1[i] != str2[i]){
			return FALSE;
		}
	}
	return TRUE;
}

uint_least8_t strcompsub(char * str1,char * str2, size_t index, size_t length){
	register uint_fast64_t i;
	if(length != strlen(str1)){
		return FALSE;
	}
	for(i = index;i<length+index;i++){
		if(str1[i-index] != str2[i]){
			return FALSE;
		}
	}
	return TRUE;
}

size_t strlength(char * str){
	if(str == NULL){
		return 0;
	}
	return strlen(str);
}

char* concat(char * s1, char * s2, uint8_t mem){
	register uint_fast64_t l1 = strlength(s1);
	register uint_fast64_t l2 = strlength(s2);
	size_t length = l1 + l2;
	char * out = (char *)malloc(sizeof(char)*(length+1));
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

char* concat_all(int args,...){
	va_list valist;
	int i = 0;
	char* output = NULL;
	va_start(valist, args);
	for(i = 0;i<args;i++){
		if(i == 0){
			output = concat(output,(char*)va_arg(valist, char*),FALSE);
		}else{
			output = concat(output,(char*)va_arg(valist, char*),FIRST);
		}
	}
	va_end(valist);
	return output;
}


int64_t strpos(char * haystack, char * needle){
	size_t length = strlen(haystack);
	size_t nlength = strlen(needle);
	register uint_fast64_t i;
	if(length == 0||nlength>length){
		return -1;
	}
	for(i = 0;i<=length-nlength;i++){
		if(haystack[i]==needle[0] && strcompsub(needle,haystack,i,nlength)){
			return i;
		}
	}
	return -1;
}
int indexOfChar(char * haystack, char needle){
	size_t length = strlen(haystack);
	register uint_fast64_t i;
	for(i = 0;i<length;i++){
		if(haystack[i]==needle){
			return i;
		}
	}
	return -1;
}

char * ktrim(char* str){
	return trim(str);
}
char * trim(char* str){
	register uint_fast64_t start = 0;
	register uint_fast64_t end = strlen(str);
	while(str[start] == '\n' || str[start]== '\r' || str[start]==' '){
		start++;
	}
	while(str[end] == '\n' || str[end]=='\r' || str[end]==' '){
		end--;
	}
	if(end-start ==0){
		printf("ERROR was found when trimming: %s\n",str);
		exit(EXIT_FAILURE);
	}
	return substring(str,start,end-start);
}


char* str_replace(char* search,char* replace,char* subject){
	int64_t start = strpos(subject,search);
	if(start == -1){
		return subject;
	}
	char* first_chunk = substring(subject,0,start);
	
	if(start+strlen(search)<strlen(subject)){
		return str_replace(search,replace,concat(first_chunk,concat(replace,substr(subject,start+strlen(search)),SECOND),FIRST|SECOND));
	}
	return str_replace(search,replace,concat(first_chunk,replace,FIRST));

}
