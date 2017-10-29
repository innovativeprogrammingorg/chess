#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <cstdint>
#include <ctime>

using namespace std;

class User{
	public:
		string username;
		bool online;
		time_t response;
		int sd;
		User() : username(""),online(false),response(time(NULL)),sd(0){}
		User(string name, int sd);
};


#endif