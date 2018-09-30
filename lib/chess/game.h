#ifndef _GAME_H_
#define _GAME_H_

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <cstdio>
#include "chess/board.h"
#include "chess/chess_types.h"
#include "user/user.h"
#include "server/WebSocket/frame.h"
#include "sql/sql.h"
#include "chess/history.h"
#include "chess/timer.h"
#include "chess/move.h"

using namespace std;

class game{
	public:
		user black;
		user white;
		board game_board;
		timer game_timer;
		int64_t id;
		history hist;
		game(User* black, User* white, int64_t id, board* b,uint8_t turn,time_t duration,int inc);
		game(User* black, User* white, int64_t id, board* b,uint8_t turn,int wtime,int btime,int last,int inc,int undo);
		game(User* black, User* white, int64_t id, board* b,Timer* t,History* hist);
	
		bool in_check(char side);
		bool is_checkmate(char side);
		bool is_draw();
		uint8_t move(int r,int c,int r2,int c2,char side);
		static bool in_check(board* b, char side);
		static bool is_protected(board* b,Location* loc,char side);
		static bool is_protected(board* b,int r,int c,char side);
		static bool tile_covered(board* b,Location* loc,char side);
		static bool tile_covered(board* b,int r,int c,char side);

};



#endif