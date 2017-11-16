#ifndef _USER_MANAGER_H_
#define _USER_MANAGER_H_


#include <string>
#include <iostream>
#include <pthread.h>
#include "user/user.h"
#include "user/user_entry.h"

using namespace std;

struct strptrcomp{
    bool operator()(string* a, string* b) const {
        return (a->compare(*b) != 0);
    }
};

class User_Manager{
	private:
		pthread_mutex_t* lock;
	public:
		map<string*,User_Entry*,strptrcomp>* data;
		User_Manager();
		virtual ~User_Manager();
	private:
		void login(string username);
		void logout(string username);
	public:
		bool has_user(string* username);
		void connect(string* username,string key,int sd);
		void connect(string* username,int64_t key,int sd);
		void disconnect(string* username,int sd);
		void drop(string* username,string key);
		void drop(string* username,int64_t key);
		int lookup(string* username,string key);
		int lookup(string* username,int64_t key);
};	

#endif