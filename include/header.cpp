#include "header.h"


using namespace std;
string get_status_line(int status){
	switch(status){
		case 101:
			return string("HTTP/1.1 101 Switching Protocols\r\n");
		case 200:
			return string("HTTP/1.1 200 OK\r\n");
		case 400:
			return string("HTTP/1.1 400 Bad Request\r\n");
		case 403:
			return string("HTTP/1.1 403 Forbidden\r\n");
		case 404:
			return string("HTTP/1.1 404 Not Found\r\n");
		case 415:
			return string("HTTP/1.1 415 Unsupported Media Type\r\n");
		case 500:
			return string("HTTP/1.1 500 Internal Server Error\r\n");
		case 503:
			return string("HTTP/1.1 503 Service Unavailable\r\n");
	}
	return string("");
}

string get_date_line(){
    time_t timer;
    char* buffer = (char*)malloc(sizeof(char)*38);
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 37, "Date: %Y-%m-%d %H:%M:%S GMT\r\n", tm_info);
    string out(buffer);
    free(buffer);
    return out;
}

string get_server_line(){
	return string("Server: Nate's C Server\r\n");
}

string get_SWA_line(HTTP_Request* r){
	string out("Sec-WebSocket-Accept: ");
	out.append(string(WS_accept(*r->header->at("Sec-WebSocket-Key")->data[0])));
	out.append("\r\n");
	return out;
}

string get_content_encoding_line(string encode){
	string out("Content-Encoding: ");
	out.append(encode);
	out.append("\r\n");
	return out;
}

string get_upgrade_line(){
	return string("Upgrade: websocket\r\n");
}

string get_content_length_line(uint64_t content_length){
	char* out = (char*)calloc(sizeof(char),100);
	sprintf(out,"Content-Length: %lu\r\n",content_length);
	string str(out);
	free(out);
	return str;
}

string get_content_type_line(string content){
	string out("Content-Type: ");
	out.append(content);
	out.append("\r\n");
	return out;
}

string get_accept_ranges_line(){
	return string("Accept-Ranges: bytes\r\n");
}

string get_connection_line(HTTP_Request* r){
	string out("Connection: ");
	out.append(*r->header->at("Connection")->data[0]);
	out.append("\r\n");
	return out;
}

uint16_t check_valid_params(HTTP_Request* r){
	if(!r->header->has_param("Upgrade")){
		cout<<"Missing Upgrade parameter"<<endl;
		return 400;
	}
	if(!r->header->has_param("Sec-WebSocket-Key")){
		cout<<"Missing Sec-WebSocket-Key parameter"<<endl;
		return 400;
	}
	if(!r->header->has_param("Sec-WebSocket-Version")){
		cout<<"Missing Sec-WebSocket-Version parameter"<<endl;
		return 500;
	}

	return TRUE;
}

uint8_t get_request_type(HTTP_Request* r){
	string* status_line = r->header->at("REQUEST")->data[0];

	if(status_line->find("GET")!=string::npos){
		return GET;
	}
	if(status_line->find("POST")!=string::npos){
		return POST;
	}
	if(status_line->find("PUT")!=string::npos){
		return PUT;
	}
	if(status_line->find("DELETE")!=string::npos){
		return DELETE;
	}
	if(status_line->find("HEAD")!=string::npos){
		return HEAD;
	}
	if(status_line->find("OPTIONS")!=string::npos){
		return OPTIONS;
	}
	if(status_line->find("CONNECT")!=string::npos){
		return CONNECT;
	}
	return UNKNOWN;
}

Response build_response(HTTP_Request* r){
	Response out;
	if(1 != check_valid_params(r)){
		out = e400_response(r);
		return out;
	}
	uint8_t request = get_request_type(r);
	switch(request){
		case GET:
			out = GET_response(r);
			break;
		case POST:
			puts("Received a POST request!");
			out = e400_response(r);
			break;
		default:
			out = e400_response(r);
			break;
	}
	return out;
}	