#include "POST.h"

using namespace std;
string parse_PHP_args(map<string*,string*> args){
	string out;
	for(map<string*,string*>::iterator it = cookies->begin();it!=cookies->end();++it){
		out += *it->first + "=" + *it->second + " ";
	}
	return out;

}
string* process_through_PHP(map<string*,string*> _POST,char* dir){
	string command = "/usr/bin/php -f " + dir + " " + parse_PHP_args(_POST);
	FILE* fd = popen(command.c_str(),"r");
	char* tmp;
	if(!fread_file(fd,&body)){
		return NULL;
	}
	string* body = new string(tmp);
	free(tmp);
	return body;
}