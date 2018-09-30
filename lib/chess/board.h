#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <cstdint>
#include "str.h"
#include "chess/chess_types.h"
#include "chess/tile.h"
#include "chess/location.h"

using namespace std;

class board{
	public:
		tile** tiles;
		uint8_t black_castle;
		uint8_t white_castle;
		char taken;
		string special;
	
		board(const string& FEN, const string& special, const string& castle);
		virtual ~board();
		tile& get_tile(int row, int col);
		bool empty(int row,int col);
		void special_data(string data);
		bool place_piece(int r,int c,char FEN);
		void force_change(int r,int c,char FEN);
		void force_move(int r,int c,int r2, int c2);
		location find_king(char side);
		piece& get_king(char side);		
		string generate_FEN();
		string get_castle_data();
		string to_string();

		static char num_to_col(int c);
		static char other_side(char side);
};


#endif