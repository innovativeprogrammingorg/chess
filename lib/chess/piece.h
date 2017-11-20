#ifndef _PIECE_H_
#define _PIECE_H_

#include <iostream>
#include "chess/chess_types.h"

using namespace std;

class Piece{
	public:
		char side;
		int name;
		char FEN;
		bool special;
		bool promo;
	
		Piece(char side,char FEN);
		bool is(int name);
		bool canSpecial();
		void setSpecial(int side);

		static int getName(char FEN);
};



#endif