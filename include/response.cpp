#include "response.h"
using namespace std;

Response new_response(string* header, string* body,size_t data_size){
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
	string* body;
	char* tmp;
	string* head;
	uint64_t content_length;
	content_length = sread_file(directory.c_str(),&tmp);
	body = new string(tmp);
	free(tmp);
	head->append(get_status_line(404));
	head->append(get_date_line());
	head->append(get_server_line());
	head->append(get_content_length_line(content_length));
	head->append(get_connection_line(r));
	head->append(get_content_type_line("text/html"));
	head->append(get_vary_line());
	//head = concat(head,get_content_encoding_line(),FIRST);
	return new_response(head,body,content_length);
}

Response e400_response(HTTP_Request* r){
	string directory = "/400.shtml";
	string* body;
	char* tmp;
	string* head = new string();
	uint64_t content_length;
	content_length = sread_file(directory,&tmp);
	body = new string(tmp);
	free(tmp);

	head->append(get_status_line(400));
	head->append(get_date_line());
	head->append(get_server_line());
	head->append(get_content_length_line(content_length));
	head->append(get_connection_line(r));
	head->append(get_content_type_line("text/html"));
	head->append(get_vary_line());
	//head = concat(head,get_content_encoding_line(),FIRST);
	return new_response(head,body,content_length);
}

Response GET_response(HTTP_Request* r){
	string directory = get_requested_directory(r);
	string* body;
	char* tmp;
	string* head = new string();
	uint64_t content_length;
	string src;
	if(directory.back()=='/'){
		directory += "index.html";
	}
	src = WEB_ROOT + directory;
	if(access(src.c_str(),F_OK)==-1){
		return e404_response(r);
	}
	string content_type = get_content_type(src);
	/**
	 * Build the HTTP Message
	 */
	head->append(get_status_line(200));
	if(content_type.find("text")==string::npos){
		head->append(get_accept_ranges_line());
		content_length = prepare_media(directory,&tmp);

		head->append(get_content_type_line(content_type));
		head->append(get_date_line());
	}else{
		content_length = sread_file(directory,&tmp);
		head->append(get_date_line());
	}
	body = new string(tmp);
	free(tmp);
	//head = concat(head,get_content_encoding_line("gzip"),FIRST);
	head->append(get_content_length_line(content_length));
	head->append(get_connection_line(r));
	head->append(get_server_line());
	//head = concat(head,get_vary_line(),FIRST);
	return new_response(head,body,content_length);
}

Response POST_response(HTTP_Request* r){
	string directory = get_requested_directory(r);
	directory = WEB_ROOT + directory;
	string* head = new string();
	string* body = new string(process_through_PHP(r->body,directory));
	if(body == nullptr){
		return nullptr;
	}
	size_t content_length = body->size();
	head->append(get_status_line(200));
	head->append(get_date_line());
	head->append(get_content_length_line(content_length));
	head->append(get_connection_line(r));
	head->append(get_server_line());
	return new_response(head,body,content_length);
}

