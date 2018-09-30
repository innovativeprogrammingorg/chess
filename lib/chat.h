#ifndef _CHAT_H_
#define _CHAT_H_

#include <vector>
#include <string>
#include <time.h>
#include <pthread.h>
#include <cstdint>
#include <map>
#include "user/user.h"
#include "server/WebSocket/frame.h"


using namespace std;

class message{
	public:
		string user;
		string msg;
		time_t date;
		explicit message(const string& user, const string& msg);
		virtual ~message();
};

class chat{
	private:
		pthread_mutex_t lock;
	public:
		vector<message> messages;
		map<User,UEKey,usercomp> users;
		chat();
		virtual ~chat();
		void add(const Message& m);
		void add(const string& user, const string& msg);
		void add(const string* user, const string* msg);
		int64_t index_of(const string& name);
		void connect(const User& user,UEKey key = nullptr);
		void connect(const string& name,const UEKey key = nullptr);
		void disconnect(const user& u);
		void disconnect(const string& name);
		void broadcast(const frame& f);
		string to_string() const;
		string get_last();
		void send_last();
		void send_all(const int sd);

};

#ifndef MAX_MESSAGES
#define MAX_MESSAGES 10
#endif



#endif