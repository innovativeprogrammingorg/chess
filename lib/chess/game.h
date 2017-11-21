#ifndef _GAME_H_
#define _GAME_H_

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include "chess/board.h"
#include "chess/chess_types.h"
#include "user/user.h"
#include "server/WebSocket/frame.h"
#include "sql/sql.h"
#include "chess/timer.h"
#include "chess/move.h"

using namespace std;

class Game{
	public:
		User* black;
		User* white;
		Board* board;
		Timer* timer;
		int64_t id;

		Game(User* black, User* white, int64_t id, Board* b,uint8_t turn,time_t duration,int inc);
		Game(User* black, User* white, int64_t id, Board* b,uint8_t turn,int wtime,int btime,int last,int inc,int undo);
		Game(User* black, User* white, int64_t id, Board* b,Timer* t);
		virtual ~Game();
		bool inCheck(char side);
		bool isCheckmate(char side);
		bool isDraw();
		uint8_t move(int r,int c,int r2,int c2,char side);
		static bool inCheck(Board* b, char side);
		static bool isProtected(Board* b,Location* loc,char side);
		static bool isProtected(Board* b,int r,int c,char side);
		static bool tileCovered(Board* b,Location* loc,char side);
		static bool tileCovered(Board* b,int r,int c,char side);

};



#endif