#ifndef _COOKIES_H_
#define _COOKIES_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <map>
#include <vector>

#include "HTTP.h"
#include "io.h"


using namespace std;

class Cookie{
	public:
		string value;
		time_t created;
		time_t expires;
		Cookie(string value = "",time_t created = 0,time_t exp = 0);
		bool store(string ip,string key);
		static bool store_cookies(string ip, map<string,Cookie*>* cookies);
		static map<string,Cookie*>* load_cookies(string ip);
		static map<string,Cookie*>* parse_cookies(string ip,HTTP_Header* r);
};




#ifndef COOKIES_DIR
#define COOKIES_DIR "./tmp/cookies/"
#endif

#ifndef COOKIES_EXP
#define COOKIES_EXP 604800 /* 1 week*/
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif