#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>
#include <iostream>
#include "../include/str.h"
#include "types.h"
#include "tile.h"
#include "json.h"

using namespace std;



class Board{
	public:
		Tile*** tiles;
		uint8_t bCastle;
		uint8_t wCastle;
		char taken;
		string special;
		vector<string*>* history;
	
		Board(string FEN, string special, string castle);
		virtual ~Board();
		Tile* getTile(int row, int col);
		//void setSpecial(string data);
		void specialData(string data);
		bool placePiece(int r,int c,char FEN);
		void forceChange(int r,int c,char FEN);
		Location* findKing(char side);
		Piece* getKing(char side);		
		string generateFEN();
		string getCastleData();
		string getBoardData();
		static char numToCol(int c);
		static char otherSide(char side);
};


#endif