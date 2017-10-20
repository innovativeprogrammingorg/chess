#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>
#include <iostream>
#include "str.h"
#include "types/chess_types.h"
#include "tile.h"

using namespace std;



class Board{
	private:
		Tile*** tiles;
		uint8_t bCastle;
		uint8_t wCastle;
		char taken;
		string special;
		Vector history;
	public:
		Board(string FEN, string special, string castle);
		virtual ~Board();
		Tile* getTile(int row, int col);
		void setSpecial(string data);
		bool placePiece(int r,int c,char FEN);
		void forceChange(int r,int c,char FEN);
		Location* findKing(char side);
		Piece* getKing(char side);
		
		
		
		
		string generateFEN();
		void forceChange(int r,int c,char FEN);
		string getCastleData();
		string getBoardData();
		void castle(Piece* p,,char side);
		static char numToCol(int c);
		static char otherSide(char side);
};


#endif