#ifndef _TILE_H_
#define _TILE_H_

#include "piece.h"

using namespace std;

class Tile{
	public:
		Piece* p;
		Tile(int row, int col,char fen);
		bool empty();
};

#endif