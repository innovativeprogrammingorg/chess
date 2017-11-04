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


using namespace std;

enum Game_Type{
	REGULAR = 0x1,
	BUG_HOUSE = 0x2
};

class Lobby_Game{
	public:
		static int64_t game_id;
		int type;
		int sec;
		int min;
		int inc;
		char side;
		string host;
		int64_t id;
		vector<User*>* players;
		int ready;
		Lobby_Game(int type,int sec,int min,int inc,char side,string host);
		virtual ~Lobby_Game();
		void add_player(User* player);
		string get_type();
		string to_string(char sep);
};

class Lobby{
	private:
		pthread_mutex_t* lock;
	public:
		vector<User*>* users;
		vector<Lobby_Game*>* games;
		Lobby();
		virtual ~Lobby();
		bool has_games();
		bool has_user(int sd);
		bool has_user(string username);
		User* get_user(string username);
		void add_user(string name, int sd);
		void add_user(User* user);
		void add_game(Lobby_Game* lg);
		void add_game(int type, int sec,int min,int inc,char side,string host);
		void add_game(vector<string>* data,string host);
		void remove_user(int sd);
		void remove_user(string username); 
		Lobby_Game* remove_game(string host);
		Lobby_Game* remove_game(int id);
		string get_users(char sep);
		string get_games(char sep);
		void broadcast(Frame* frame);


};

#endif