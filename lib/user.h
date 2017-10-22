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
		User(string name);

};


#endif