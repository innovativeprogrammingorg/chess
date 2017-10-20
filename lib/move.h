#ifndef MOVE_H
#define MOVE_H

#include <cstdlib>
#include <vector>
#include "piece.h"
#include "location.h"
#include "types/types.h"
#include "board.h"


class Move{
	public:
		int r1;
		int c1;
		int c2;
		int r2;
		Move(int r1,int c1,int r2,int c2) : r1(r1),c1(c1),r2(r2),c2(c2);

		static bool validMove(Piece* p,Location* move,Board* b);
		static void castle(Piece* p,Board* b,char side);
		static char specialMove(Piece* p,Location* move,Board* b);
		static bool valid_king(Piece* p, Location* move, Board* b);
		static bool valid_queen(Piece* p,Location* move,Board* b);
		static bool valid_pawn(Piece* p, Location* move,Board* b);
		static bool valid_bishop(Piece* p,Location* move,Board* b);
		static bool valid_knight(Piece* p,Location* move,Board* b);
		static bool valid_rook(Piece* p,Location* move,Board* b);
		


};



#endif
