#ifndef _GAME_H_
#define _GAME_H_

#include "board.h"
#include "move.h"
#include "str.h"

using namespace std;

class Game{
	public:
		Board* board;
		Game();
		bool inCheck(char side);
		bool isCheckmate(char side);
		bool isDraw();
		bool move(int r,int c,int r2,int c2,char side);
		static bool inCheck(Board* b, char* side);
		static bool isProtected(Location* loc,Board* b,char side);
		static bool tileCovered(Board* b,Location loc,char side);
		static bool tileCovered(Board* b,int r,int c,char side);
}


#endif