#ifndef _LOBBY_GAME_H_
#define _LOBBY_GAME_H_

#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include "game.h"
#include "../include/str.h"
#include "user.h"

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
		string host;
		int64_t id;
		vector<User*>* players;
		int ready;
		int seats;

		Lobby_Game(int type,int sec,int min,int inc,string host);
		virtual ~Lobby_Game();
		char get_open_side();
		void add_player(User* player);
		void add_player(string username);
		string get_type();
		string to_string(char sep);
		bool is_ready();
		int spots_open();
		void drop_player(string user);
		Game* create_game();
};


#endif