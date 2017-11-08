#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <cstdint>
#include <ctime>
#include "../include/client.h"

using namespace std;

class User{
	public:
		string username;
		bool online;
		time_t response;
		char side;
		User() : username(""),online(false),response(time(NULL)){}
		User(string name);
		User(string name, char side);
		int sd();
};


#endif