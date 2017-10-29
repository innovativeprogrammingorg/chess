#include "POST.h"

using namespace std;
string parse_PHP_args(map<string,string*>* args){
	string out;
	for(map<string,string*>::iterator it = args->begin();it!=args->end();++it){
		out += it->first + "=" + *it->second + " ";
	}
	return out;

}
string* process_through_PHP(map<string,string*>* _POST,string dir){
	string command = "/usr/bin/php -f " + dir + " " + parse_PHP_args(_POST);
	FILE* fd = popen(command.c_str(),"r");
	char* tmp;
	if(!IO::read_file(fd,&tmp)){
		return NULL;
	}
	string* body = new string(tmp);
	free(tmp);
	return body;
}