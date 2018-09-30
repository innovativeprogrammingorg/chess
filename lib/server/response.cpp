#include "server/response.h"
using namespace std;

Response new_response(string* header, char* body,size_t data_size){
	Response out = (Response)malloc(sizeof(struct response));
	out->header = header;
	out->body = body;
	out->data_size = data_size;
	return out;
}


Response GET_response(HTTP_Request* r){
	string* head = new string();
	/**
	 * Build the HTTP Message
	 */
	head->append(get_status_line(101));
	head->append(get_upgrade_line());
	head->append(get_connection_line(r));
	head->append(get_SWA_line(r));
	head->append("\r\n");
	return new_response(head,nullptr,0);
}