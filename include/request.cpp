#include "request.h"

using namespace std;

Request new_request(void* c, char* data){
	Request out = (Request)malloc(sizeof(struct request));
	out->client = c;
	out->data = data;
	return out;
}



bool is_PHP_request(string message, string* directory){
	size_t i = message.find("\r\n");
	if(i == string::npos){
		cout<<"Unexpected error in is_PHP_request"<<endl;
		exit(EXIT_FAILURE);
	}
	string first_line = message.substr(0,i);
	size_t start = first_line.find('/');
	int end = 0;
	while(first_line[end+start]!=' ' && first_line[end+start]!='\0' && first_line[end+start]!='?'){
		end++;
	}
	string dir = "./www" + first_line.substr(start,end);
	string type = IO::get_content_type(dir);
	if(type.find("php")!= string::npos){
		*directory = dir;
		return true;
	}
	*directory = "";
	return false;
}
