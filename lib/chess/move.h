#ifndef MOVE_H
#define MOVE_H

#include <cstdlib>
#include <vector>
#include "piece.h"
#include "location.h"
#include "chess_types.h"
#include "board.h"
#include "game.h"

using namespace std;

class Move{
	public:
		int r1;
		int c1;
		int c2;
		int r2;
		Move(int r1,int c1,int r2,int c2) : r1(r1),c1(c1),r2(r2),c2(c2) {}

	
		static void castle(Board* b,char side,char player);

		static char specialMove(Piece* p,Move* move,Board* b);
		static char specialMove(Piece* p,int r,int c,int mr,int mc,Board* b);

		static bool valid_king(Piece* p, Move* move, Board* b);
		static bool valid_king(Piece* p,int r,int c,int mr,int mc, Board* b);

		static bool valid_queen(Piece* p,Move* move,Board* b);
		static bool valid_queen(Piece* p,int r,int c,int mr,int mc,Board* b);

		static bool valid_pawn(Piece* p, Move* move,Board* b);
		static bool valid_pawn(Piece* p,int r,int c,int mr,int mc,Board* b);

		static bool valid_bishop(Piece* p,Move* move,Board* b);
		static bool valid_bishop(Piece* p,int r,int c,int mr,int mc,Board* b);

		static bool valid_knight(Piece* p,Move* move,Board* b);
		static bool valid_knight(Piece* p,int r,int c,int mr,int mc,Board* b);

		static bool valid_rook(Piece* p,Move* move,Board* b);
		static bool valid_rook(Piece* p,int r,int c,int mr,int mc,Board* b);

		static bool validMove(Piece* p,Move* move,Board* b);

		static bool valid(Piece* p,Move* move,Board* b);
		static bool valid(Piece* p,int r,int c,int mr,int mc,Board* b);

};



#endif
