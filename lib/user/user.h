#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <cstdint>
#include "server/client.h"
#include "user/user_manager.h"
#include "user/user_entry.h"

using namespace std;


class User{
	public:
		string* username;
		char side;
		User(string name);
		User(string name, char side);
		virtual ~User();
		int sd();
		int sd(string key);
		int sd(int64_t key);
		int sd(UEKey key);

};

struct usercomp{
    bool operator()(User* a, User* b) const {
        return ( a->username->compare(*b->username)!= 0);
    }
};


#endif