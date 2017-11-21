#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>
#include "str.h"
#include "chess/chess_types.h"
#include "chess/tile.h"
#include "chess/location.h"

using namespace std;

class Board{
	public:
		Tile*** tiles;
		uint8_t bCastle;
		uint8_t wCastle;
		char taken;
		string special;
	
		Board(string FEN, string special, string castle);
		virtual ~Board();
		Tile* getTile(int row, int col);
		void specialData(string data);
		bool placePiece(int r,int c,char FEN);
		void forceChange(int r,int c,char FEN);
		void forceMove(int r,int c,int r2, int c2);
		Location* findKing(char side);
		Piece* getKing(char side);		
		string generateFEN();
		string getCastleData();
		string to_string();
		static char numToCol(int c);
		static char otherSide(char side);
};


#endif