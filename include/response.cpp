#include "response.h"
using namespace std;

Response new_response(string* header, char* body,size_t data_size){
	Response out = (Response)malloc(sizeof(struct response));
	out->header = header;
	out->body = body;
	out->data_size = data_size;
	return out;
}

string get_requested_directory(HTTP_Request* r){
	string status_line = *r->header->at("REQUEST")->data[0];
	int start = status_line.find("/");
	int end = 0;
	while(status_line[end+start]!=' ' && status_line[end+start]!='\0' && status_line[end+start]!='?'){
		end++;
	}
	return status_line.substr(start,end);
}

Response e404_response(HTTP_Request* r){
	string directory = "/404.shtml";
	char* body;
	string* head;
	uint64_t content_length;
	content_length = IO::read_file((char*)directory.c_str(),&body);
	head->append(get_status_line(404));
	head->append(get_date_line());
	head->append(get_server_line());
	head->append(get_content_length_line(content_length));
	head->append(get_connection_line(r));
	head->append(get_content_type_line("text/html"));
	//head = concat(head,get_content_encoding_line(),FIRST);
	return new_response(head,body,content_length);
}

Response e400_response(HTTP_Request* r){
	string directory = "/400.shtml";
	char* body;
	char* tmp;
	string* head = new string();
	uint64_t content_length;
	content_length = IO::read_file((char*)directory.c_str(),&body);
	head->append(get_status_line(400));
	head->append(get_date_line());
	head->append(get_server_line());
	head->append(get_content_length_line(content_length));
	head->append(get_connection_line(r));
	head->append(get_content_type_line("text/html"));
	//head = concat(head,get_content_encoding_line(),FIRST);
	return new_response(head,body,content_length);
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

Response POST_response(HTTP_Request* r){
	string directory = get_requested_directory(r);
	directory = WEB_ROOT + directory;
	string* head = new string();
	string* body = process_through_PHP(r->body,directory);
	if(body == nullptr){
		return nullptr;
	}
	size_t content_length = body->size();
	head->append(get_status_line(200));
	head->append(get_date_line());
	head->append(get_content_length_line(content_length));
	head->append(get_connection_line(r));
	head->append(get_server_line());
	char* cbody = (char*)calloc(sizeof(char),body->size()+1);
	memcpy(cbody,body->c_str(),body->size()* sizeof(char));
	delete body;
	return new_response(head,cbody,content_length);
}

