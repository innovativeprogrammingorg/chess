#ifndef _GAME_H_
#define _GAME_H_

#include "board.h"
#include "../include/str.h"
#include "types.h"
#include "user.h"
#include <time.h>
#include "../include/WebSocket/frame.h"
#include "sql/sql.h"


using namespace std;

class Game{
	public:
		User* black;
		User* white;
		Board* board;
		uint8_t turn;
		time_t last_move_time;
		uint64_t inc;
		time_t white_time;
		time_t black_time;
		uint64_t id;
		uint64_t turns;
		vector<string>* history;

		Game();
		Game(User* black, User* white, uint64_t id, Board* b,uint8_t turn,time_t duration,uint64_t inc);
		virtual ~Game();
		bool inCheck(char side);
		bool isCheckmate(char side);
		bool isDraw();
		uint8_t move(int r,int c,int r2,int c2,char side);
		void store();
		bool send_state_to_opponent(int sd);
		bool send_state(int sd);
		static bool inCheck(Board* b, char side);
		static bool isProtected(Location* loc,Board* b,char side);
		static bool tileCovered(Board* b,Location* loc,char side);
		static bool tileCovered(Board* b,int r,int c,char side);

};

#include "move.h"

#endif