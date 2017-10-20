#ifndef _PIECE_H_
#define _PIECE_H_


#include "location.h"
#include "types/chess_types.h"
#include <iostream>


using namespace std;

class Piece{
	public:
		Location* loc;
		char side;
		int name;
		char FEN;
		bool special;
		bool promo;
	
		Piece(Location* loc,char side,char FEN);
		bool is(int name);
		bool canSpecial();
		void setSpecial(int side);

		void move(Location* loc);
		static int getName(char FEN);
};



#endif