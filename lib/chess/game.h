#ifndef _GAME_H_
#define _GAME_H_

#include <time.h>
#include <cstdint>
#include <vector>
#include <string>
#include "chess/board.h"
//#include "str.h"
#include "chess/chess_types.h"
#include "user/user.h"
#include "server/WebSocket/frame.h"
#include "sql/sql.h"


using namespace std;

class Game{
	public:
		User* black;
		User* white;
		Board* board;
		uint8_t turn;
		int last_move_time;
		int inc;
		int white_time;
		int black_time;
		int64_t id;
		int turns;
		vector<string>* history;

		Game();
		Game(User* black, User* white, int64_t id, Board* b,uint8_t turn,time_t duration,int inc);
		Game(User* black, User* white, int64_t id, Board* b,uint8_t turn,int wtime,int btime,int last,int turns,int inc);
		virtual ~Game();
		bool inCheck(char side);
		bool isCheckmate(char side);
		bool isDraw();
		uint8_t move(int r,int c,int r2,int c2,char side);
		bool send_state_to_opponent(int sd);
		bool send_state(int sd);
		static bool inCheck(Board* b, char side);
		static bool isProtected(Location* loc,Board* b,char side);
		static bool tileCovered(Board* b,Location* loc,char side);
		static bool tileCovered(Board* b,int r,int c,char side);

};

#include "move.h"

#endif