#ifndef _CHAT_H_
#define _CHAT_H_

#include <vector>
#include <string>
#include <time.h>
#include <pthread.h>
#include <cstdint>
#include "../include/WebSocket/frame.h"


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
		vector<int>* users;
		Chat();
		virtual ~Chat();
		void add(Message* m);
		void add(string user, string msg);
		void add(string* user, string* msg);
		int64_t indexOf(int sd);
		void connect(int sd);
		void disconnect(int sd);
		void broadcast(Frame* frame, int sd = 0);
		string to_string(char sep);

};

#ifndef MAX_MESSAGES
#define MAX_MESSAGES 40
#endif

#endif