#ifndef _GAME_H_
#define _GAME_H_

#include "board.h"
#include "str.h"
#include "types.h"
#include "user.h"

using namespace std;

class Game{
	public:
		User* black;
		User* white;
		Board* board;
		uint8_t turn;
		uint64_t start;
		uint64_t duration;
		uint64_t inc;
		uint64_t white_time;
		uint64_t black_time;
		uint64_t id;
		uint64_t turns;

		Game();
		Game(User* black, User* white, uint64_t id, Board* b,uint8_t turn,time_t duration,uint64_t inc);
		virtual ~Game();
		bool inCheck(char side);
		bool isCheckmate(char side);
		bool isDraw();
		uint8_t move(int r,int c,int r2,int c2,char side);
		void store();

		static bool inCheck(Board* b, char side);
		static bool isProtected(Location* loc,Board* b,char side);
		static bool tileCovered(Board* b,Location* loc,char side);
		static bool tileCovered(Board* b,int r,int c,char side);

};

#include "move.h"

#endif