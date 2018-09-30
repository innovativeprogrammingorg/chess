#ifndef _TILE_H_
#define _TILE_H_

#include "chess/piece.h"

using namespace std;

class Tile{
	public:
		piece p;
		tile(char fen);
		void change(char fen);
		void change(const piece& p);
		bool empty();
		void clear();
};

#endif