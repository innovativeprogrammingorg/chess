#ifndef _TILE_H_
#define _TILE_H_

#include "chess/piece.h"

using namespace std;

class Tile{
	public:
		Piece* p;
		bool is_empty;
		Tile(char fen);
		void change(char fen);
		virtual ~Tile();
		bool empty();
};

#endif