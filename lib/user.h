#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <cstdint>
#include <ctime>
#include "../include/client.h"

using namespace std;

class User{
	public:
		string* username;
		bool online;
		time_t response;
		char side;
		User(string name);
		User(string name, char side);
		virtual ~User();
		int sd();
};


#endif