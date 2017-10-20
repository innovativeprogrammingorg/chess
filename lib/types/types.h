#ifndef TYPES_H
#define TYPES_H

#include "chess_types.h"

typedef struct game* Game;
typedef struct user* User;
typedef struct client* Client;

struct game{
	User black;
	User white;
	Board b;
	uint8_t turn;
	int64_t turns;
	time_t duration;
	time_t white_time;
	time_t black_time;
	uint64_t inc;
	uint64_t start;
	uint64_t id;
};

struct user{
	char* username;
	uint8_t online;
	time_t response;
};

struct client{
	uint32_t addr;
	in_port_t port;
	uint32_t sd;
};



#endif