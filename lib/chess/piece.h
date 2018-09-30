#ifndef _PIECE_H_
#define _PIECE_H_

#include <iostream>
#include "chess/chess_types.h"

using namespace std;

class piece{
	public:
		char side;
		char FEN;
		int name;
		bool special;
		bool promo;
		
		piece();
		piece(char side,char FEN);
		virtual ~piece();
		bool is(int name) const;
		bool can_special() const;
		void set_special(int side);
		void change(char side,char FEN);
		bool empty();
		void clear();

		static int get_name(char FEN);
};



#endif