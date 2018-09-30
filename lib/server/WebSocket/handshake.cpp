#include "server/WebSocket/handshake.h"
using namespace std;

char* WS_accept(string key){
	string tmp = key;
	tmp += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	uint8_t* out = (uint8_t*)calloc(sizeof(uint8_t),21);
	out[20] = '\0';
	SHA1((unsigned char*)tmp.c_str(), tmp.size(), out);
	char* b64 = b64_encode(out,20);
	free(out);
	return b64;
}