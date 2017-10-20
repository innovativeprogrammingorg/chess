#include "base64.h"

char convertValue(uint8_t a){
	if(a<26){
		return 65 + a;
	}
	if(a<52){
		return 97 + (a-26);
	}
	if(a<62){
		return 48 + (a-52);
	}
	return (a==62)? '+' : '/';

}
char* convertToBinary(char* input){
	size_t length = strlen(input);
	char * out = calloc(sizeof(char),length*8 + 1);
	uint64_t i = 0;
	uint8_t tmp;
	for(i = 0;i<length;i++){
		tmp = (uint8_t)input[i];
		out[8*i    ] = (tmp & 128)? '1' : '0';
		out[8*i + 1] = (tmp &  64)? '1' : '0';
		out[8*i + 2] = (tmp &  32)? '1' : '0';
		out[8*i + 3] = (tmp &  16)? '1' : '0';
		out[8*i + 4] = (tmp &   8)? '1' : '0';
		out[8*i + 5] = (tmp &   4)? '1' : '0';
		out[8*i + 6] = (tmp &   2)? '1' : '0';
		out[8*i + 7] = (tmp &   1)? '1' : '0';
	}
	return out;
}
char* convertToBase64(char* input){
	char* binary = convertToBinary(input);
	size_t output_size = strlen(binary)/6 + ((strlen(binary)%6 == 0)? 0 : 1);
	uint64_t i = 0;
	char* out = (char*)calloc(sizeof(char),output_size + 3);
	uint8_t tmp = 0;
	uint8_t left_over = 0;
	for(i = 0;i<output_size;i++){
		tmp += (binary[i*6    ] == '1')? 32 : 0;
		tmp += (binary[i*6 + 1] == '1')? 16 : 0;
		tmp += (binary[i*6 + 2] == '1')?  8 : 0;
		tmp += (binary[i*6 + 3] == '1')?  4 : 0;
		tmp += (binary[i*6 + 4] == '1')?  2 : 0;
		tmp += (binary[i*6 + 5] == '1')?  1 : 0;
		out[i] = convertValue(tmp);
		//printf("%c",out[i]);
		tmp = 0;
	}
	left_over = (uint8_t)(strlen(binary)%6);
	if(left_over != 0){
		out[output_size] = '=';
		if(left_over == 2){
			out[output_size+1] = '=';
		}
	}
	free(binary);
	return out;
}