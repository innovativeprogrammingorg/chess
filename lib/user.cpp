#include "user.h"

using namespace std;

User::User(string name){
	this->username = name;
	this->online = true;
	this->response = time(NULL);
	this->side = 'w';
}

User::User(string name,char side){
	this->username = name;
	this->online = true;
	this->response = time(NULL);
	this->side = side;
}

int User::sd(){
	Client* c = Client::find_client(this->username);
	if(c==nullptr){
		return -1;
	}
	return c->sd;
}
