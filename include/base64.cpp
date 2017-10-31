#include "base64.h"

using namespace std;
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
char* convertToBinary(char* input,uint64_t size){
	size_t length = size;
	if(size == 0){
		length = strlen(input);
	}
	char * out = (char*)calloc(sizeof(char),length*8 + 1);
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


char* b64_encode(uint8_t* input,size_t length){
	size_t size = length;
	if(length == 0){
		size = strlen((char*)input);
	}	
	
	char* out = (char*)malloc(sizeof(char)*(size/3)*4 + 1);
	uint64_t loc = 0;
	uint64_t i;
	for(i = 0;i<size/3;i++){
		//out = (char*)realloc((void*)out,loc + 5);
		out[loc++] = convertValue((input[3*i] & (0b111111<<2))>>2);
		out[loc++] = convertValue(((input[3*i] & 0b11)<<4) + ((input[3*i + 1] & (0b1111<<4))>>4));
		out[loc++] = convertValue( ((input[3*i + 1] & 0b1111)<<2) + ((input[3*i + 2] & (0b11<<6))>>6));
		out[loc++] = convertValue((input[3*i + 2] & 0b111111));
	}
	if(size%3 != 0){
		out = (char*)realloc((void*)out,loc+5);
		out[loc++] = convertValue((input[3*i] & (0b111111<<2))>>2);
		if(size%3 == 2){
			out[loc++] = convertValue(((input[3*i] & 0b11)<<4) + ((input[3*i + 1] & (0b1111<<4))>>4));
			out[loc++] = convertValue( ((input[3*i + 1] & 0b1111)<<2));
		}else{
			out[loc++] = convertValue(((input[3*i] & 0b11)<<4));
			out[loc++] = '=';
			
		}
		out[loc++] = '=';
	}
	out[loc] = '\0';
	return out;
}

uint8_t base64_value(char a){

	if(a > 64 && a < 81){
		return a - 65;
	}
	if(a>96 && a<123){
		return a - 71;
	}
	if(a>=48 && a<=58){
		return a + 4;
	}

	return (a=='+')? 62 : 63;
}

uint8_t* b64_decode(const char* src, size_t len, size_t* decsize) {
	int i = 0;
	int j = 0;
	int l = 0;
	size_t size = 0;
	uint8_t *dec = NULL;
	uint8_t buf[3];
	uint8_t tmp[4];

	// alloc
	dec = (uint8_t *) malloc(sizeof(uint8_t));
	

	// parse until end of source
	while (len--) {
	// break if char is `=' or not base64 char
		if ('=' == src[j]) { 
			break; 
		}
		if (!(isalnum(src[j]) || '+' == src[j] || '/' == src[j])) { break; }

		// read up to 4 bytes at a time into `tmp'
		tmp[i++] = src[j++];

		// if 4 bytes read then decode into `buf'
		if (4 == i) {
		  // translate values in `tmp' from table
		  for (i = 0; i < 4; ++i) {
		    // find translation char in `b64_table'
		    for (l = 0; l < 64; ++l) {
		      	if (tmp[i] == convertValue(l)) {
		        	tmp[i] = l;
		        	break;
		      	}
		    }
		  }

		  // decode
		buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
		buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
		buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

		  // write decoded buffer to `dec'
		dec = (uint8_t *)realloc(dec,size + 3);
		if (dec != NULL){
		    for (i = 0; i < 3; ++i) {
		      dec[size++] = buf[i];
		    }
		}else{
		   	return NULL;
		}

		  // reset
		  i = 0;
		}
	}

	// remainder
	if (i > 0) {
	// fill `tmp' with `\0' at most 4 times
		for (j = i; j < 4; ++j) {
		  	tmp[j] = '\0';
		}

		// translate remainder
		for (j = 0; j < 4; ++j) {
		    // find translation char in `b64_table'
		    for (l = 0; l < 64; ++l) {
		      	if (tmp[j] == convertValue(l)) {
		        	tmp[j] = l;
		        	break;
		      	}
		    }
		}

		// decode remainder
		buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
		buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
		buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

		// write remainer decoded buffer to `dec'
		dec = (uint8_t *) realloc(dec, size + (i - 1));
		if (dec != NULL){
			for (j = 0; (j < i - 1); ++j) {
				dec[size++] = buf[j];
			}
		}else{
			return NULL;
		}
	}

	// Make sure we have enough space to add '\0' character at end.
	dec = (uint8_t *) realloc(dec, size + 1);
	if (dec != NULL){
		dec[size] = '\0';
	}else{
		return NULL;
	}

	// Return back the size of decoded string if demanded.
	if (decsize != NULL) {
		*decsize = size;
	}

	return dec;
}