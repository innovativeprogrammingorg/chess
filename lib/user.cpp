#include "user.h"

using namespace std;

User::User(string name, int sd){
	this->username = name;
	this->online = false;
	this->response = time(NULL);
	this->sd = sd;
}


