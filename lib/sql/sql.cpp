#include "sql.h"


using namespace std;

string parse_PHP_args(map<string,string>* args){
	string out = "";

	uint64_t i = 0;
	for(auto it = args->begin();it != args->end();it++){
		out += it->first + "=" + it->second + " ";
	}
	return out;

}
string process_through_PHP(map<string,string>* _POST,string dir){

	string command = "/usr/bin/php -f " + dir + " " + parse_PHP_args(_POST);
	FILE* fd = popen(command.c_str(),"r");
	char* body;
	if(!IO::read_file(fd,&body)){
		return NULL;
	}
	string out(body);
	free(body);
	return out;
}

string get_game_info(map<string,string>* data){
	return process_through_PHP(data,"./lib/sql/getGame.php");
}