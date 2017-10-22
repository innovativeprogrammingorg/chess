#include "user.h"

using namespace std;

User::User(string name){
	this->username = name;
	this->online = false;
	this->response = time(NULL);
}
