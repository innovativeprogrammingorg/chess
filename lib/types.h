#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <sys/time.h>
#include "vector.h"

typedef struct user* User;
typedef struct game* Game;
typedef struct board* Board;
typedef struct tile* Tile;
typedef struct piece* Piece;
typedef struct location* Location;

typedef struct client* Client;

struct user{
	char* username;
	uint8_t online;
	time_t response;
	uint8_t undo;
};

struct game{
	User black;
	User white;
	Board b;
	uint8_t turn;
	time_t start;
	time_t duration;
	uint64_t inc;
	uint64_t white_time;
	uint64_t black_time;
	uint64_t id;
	uint64_t turns;
};

struct board{
	Tile** tiles;
	uint8_t bCastle;
	uint8_t wCastle;
	char taken;
	char* special;
	Vector history;
};

struct location{
	int col;
	int row;
};

struct piece{
	Location loc;
	char side;
	uint16_t name;
	char FEN;
	uint8_t special;
	uint8_t promo;
};

struct client{
	uint32_t addr;
	in_port_t port;
	uint32_t sd;
};


struct tile{
	Piece p;
};
#define KING_SIDE 2
#define QUEEN_SIDE 1
#define PROMOTION 64
#define WHITE 'w'
#define BLACK 'b'

#define RIGHT 1
#define LEFT 2

#define PAWN 0x01
#define KNIGHT 0x02
#define BISHOP 0x04
#define ROOK 0x08
#define QUEEN 0x010
#define KING 0x020
#define PROMOTED_PAWN 0x040

#define BLACK_PAWN 'p'
#define WHITE_PAWN 'P'
#define BLACK_KNIGHT 'n'
#define WHITE_KNIGHT 'N'
#define BLACK_BISHOP 'b'
#define WHITE_BISHOP 'B'
#define BLACK_ROOK 'r'
#define WHITE_ROOK 'R'
#define BLACK_QUEEN 'q'
#define WHITE_QUEEN 'Q'
#define BLACK_KING 'k'
#define WHITE_KING 'K'

#define EMPTY_SPACE 'X'

#define PROMOTED_BLACK_KNIGHT 'o'
#define PROMOTED_WHITE_KNIGHT 'O'
#define PROMOTED_BLACK_BISHOP 'a'
#define PROMOTED_WHITE_BISHOP 'A'
#define PROMOTED_BLACK_ROOK 's'
#define PROMOTED_WHITE_ROOK 'S'
#define PROMOTED_BLACK_QUEEN 't'
#define PROMOTED_WHITE_QUEEN 'T'

#endif