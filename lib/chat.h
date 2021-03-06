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

class Message{
	public:
		string* user;
		string* message;
		time_t date;
		Message(string* user, string* message);
		virtual ~Message();
};

class Chat{
	private:
		pthread_mutex_t* lock;
	public:
		vector<Message*>* messages;
		map<User*,UEKey,usercomp>* users;
		Chat();
		virtual ~Chat();
		void add(Message* m);
		void add(string user, string msg);
		void add(string* user, string* msg);
		int64_t indexOf(string name);
		void connect(User* user,UEKey key = nullptr);
		void connect(string name,UEKey key = nullptr);
		void disconnect(User* user);
		void disconnect(string name);
		void broadcast(Frame* frame);
		string to_string();
		string get_last();
		void send_last();
		void send_all(int sd);

};

#ifndef MAX_MESSAGES
#define MAX_MESSAGES 10
#endif



#endif