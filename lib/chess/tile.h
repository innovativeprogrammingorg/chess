#ifndef _TILE_H_
#define _TILE_H_

#include "chess/piece.h"

using namespace std;

class Tile{
	public:
		Piece* p;
		Tile(char fen);
		virtual ~Tile();
		bool empty();
};

#endif