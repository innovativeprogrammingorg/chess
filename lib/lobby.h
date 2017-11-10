#ifndef _LOBBY_H_
#define _LOBBY_H_

#include <pthread.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include "user.h"
#include "../include/str.h"
#include "../include/WebSocket/frame.h"

#include "lobby_game.h"


using namespace std;


class Lobby{
	private:
		pthread_mutex_t* lock;
	public:
		vector<User*>* users;
		vector<Lobby_Game*>* games;
		Lobby();
		virtual ~Lobby();
		bool has_games();
		bool has_user(string username);
		User* get_user(string username);
		void add_user(string name);
		void add_user(User* user);
		void add_game(Lobby_Game* lg);
		void add_game(int type, int sec,int min,int inc,char side,string host);
		void add_game(vector<string>* data,string host);
		Lobby_Game* get_game(int64_t id);
		void remove_user(string username); 
		Lobby_Game* remove_game(string host);
		Lobby_Game* remove_game(int id);
		string get_users();
		string get_games();
		void broadcast(Frame* frame);
		void notify_users();
		void notify_games();
		void notify();
	private:
		void check();


};

#endif